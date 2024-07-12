#include "Renderer.h"
#include "Actor.h"
#include "Logger.h"
#include "Viewer.h"
#include "Window.h"
#include <cmath>
#include <numeric>

using namespace Jelly;

void Renderer::Render(const vk::raii::CommandBuffer& commandBuffer) noexcept
{
    Logger::GetInstance()->Trace();

    auto viewer = m_viewer.lock(); // 如果 Renderer 存在，Viewer 一定存在，此处无需判空

    auto offset = vk::Offset2D {
        static_cast<int32_t>(viewer->GetExtent().width * m_viewport[0]),
        static_cast<int32_t>(viewer->GetExtent().height * m_viewport[1])
    };
    auto extent = vk::Extent2D {
        static_cast<uint32_t>(viewer->GetExtent().width * m_viewport[2]),
        static_cast<uint32_t>(viewer->GetExtent().height * m_viewport[3])
    };
    auto viewport = vk::Viewport {
        static_cast<float>(offset.x),
        static_cast<float>(offset.y),
        static_cast<float>(extent.width),
        static_cast<float>(extent.height),
        0.0f,
        1.0f
    };

    m_camera->SetAspectRatio(static_cast<double>(extent.width) / static_cast<double>(extent.height));

    vk::ClearValue clearValue {m_background};
    vk::ClearAttachment attachment {vk::ImageAspectFlagBits::eColor, 0, clearValue};
    vk::ClearRect rect {
        vk::Rect2D {offset, extent},
         0, 1
    };

    commandBuffer.setViewport(0, viewport);
    commandBuffer.setScissor(0, vk::Rect2D(offset, extent));
    commandBuffer.clearAttachments(attachment, rect);

    for (auto& actor : m_actors)
    {
        if (actor->GetVisibility())
        {
            actor->Render(commandBuffer, viewer, this);
        }
    }
}

void Renderer::AddActor(std::shared_ptr<Actor> actor)
{
    if (m_device)
    {
        actor->SetDevice(m_device);
    }

    m_actors.emplace_back(std::move(actor));
}

void Renderer::SetDevice(std::shared_ptr<Device> device) noexcept
{
    m_device = std::move(device);
    for (auto& actor : m_actors)
    {
        actor->SetDevice(m_device);
    }
}

void Renderer::SetViewer(std::shared_ptr<Viewer> viewer) noexcept
{
    m_viewer = std::move(viewer);
}

void Renderer::SetViewport(const std::array<double, 4>& viewport)
{
    m_viewport = viewport;
}

void Renderer::SetBackground(const std::array<float, 4>& background)
{
    m_background = background;
}

void Renderer::ResetCamera() const noexcept
{
    auto bounds = GetVisibleActorBounds();

    std::array center = {
        std::midpoint(bounds[0], bounds[1]), std::midpoint(bounds[2], bounds[3]), std::midpoint(bounds[4], bounds[5])
    };

    auto radius = std::hypot(bounds[1] - bounds[0], bounds[3] - bounds[2], bounds[5] - bounds[4]) * 0.5;

    // 如果仅仅只有一个点，将半径设置为 0.5
    radius = radius == 0.0 ? 0.5 : radius;

    auto distance = radius / std::sin(m_camera->GetViewAngle() * 0.5);
    auto vpNormal = m_camera->GetViewPlaneNormal();

    m_camera->SetFocalPos(center);
    m_camera->SetEyePos(
        {center[0] + distance * vpNormal[0], center[1] + distance * vpNormal[1], center[2] + distance * vpNormal[2]}
    );

    m_camera->SetOrthographicScale(radius);

    static constexpr double expandRatio {1.1}; // 留出来一点空间，不要让前后裁剪平面紧贴着包围盒球体
    m_camera->SetClipRange({distance - radius * expandRatio, distance + radius * expandRatio});
}

void Renderer::ResetCameraClipRange() const noexcept
{
    auto bounds   = GetVisibleActorBounds();
    auto vpNormal = m_camera->GetViewPlaneNormal();
    auto& eyePos  = m_camera->GetEyePos();

    auto a = -vpNormal[0];
    auto b = -vpNormal[1];
    auto c = -vpNormal[2];
    auto d = -(a * eyePos[0] + b * eyePos[1] + c * eyePos[2]);

    std::array<double, 2> range {};

    range[0] = a * bounds[0] + b * bounds[2] + c * bounds[4] + d;
    range[1] = std::numeric_limits<double>::min();

    for (auto k = 0u; k < 2; ++k)
    {
        for (auto j = 0u; j < 2; ++j)
        {
            for (auto i = 0u; i < 2; ++i)
            {
                auto dist = a * bounds[i] + b * bounds[2 + j] + c * bounds[4 + k] + d;
                range[0]  = range[0] > dist ? dist : range[0];
                range[1]  = range[1] < dist ? dist : range[1];
            }
        }
    }

    static constexpr double expandRadio {0.1}; // 留出来一点空间，不要让前后裁剪平面紧贴着包围盒球体
    range[0] = range[0] - range[0] * expandRadio;
    range[1] = range[1] + range[1] * expandRadio;

    // 保证近裁剪面在相机的前面
    if (range[0] <= 0.0)
    {
        static constexpr double nearClipPlaneTolerance {0.01}; // 可以根据深度缓冲的字节数调整
        range[0] = range[1] * nearClipPlaneTolerance;
    }

    m_camera->SetClipRange(range);
}

std::shared_ptr<Camera> Renderer::GetCamera() const noexcept
{
    return m_camera;
}

std::array<double, 6> Renderer::GetVisibleActorBounds() const noexcept
{
    std::array<double, 6> allBounds {
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::lowest(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::lowest(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::lowest()
    };

    bool nothingVisivle {true};
    for (const auto& actor : m_actors)
    {
        if (actor->GetVisibility())
        {
            nothingVisivle = false;

            auto bounds  = actor->GetBounds();
            allBounds[0] = std::min(bounds[0], allBounds[0]);
            allBounds[1] = std::max(bounds[1], allBounds[1]);
            allBounds[2] = std::min(bounds[2], allBounds[2]);
            allBounds[3] = std::max(bounds[3], allBounds[3]);
            allBounds[4] = std::min(bounds[4], allBounds[4]);
            allBounds[5] = std::max(bounds[5], allBounds[5]);
        }
    }

    if (nothingVisivle)
    {
        allBounds = {-1., 1., -1., 1., -1., 1.};
    }

    return allBounds;
}

bool Renderer::IsInViewport(const std::array<int, 2>& position) const noexcept
{
    if (position[0] < 0 || position[1] < 0)
    {
        return false;
    }

    if (auto viewer = m_viewer.lock())
    {
        auto&& extent = viewer->GetExtent();

        auto startX = static_cast<int>(extent.width * m_viewport[0]);
        auto startY = static_cast<int>(extent.height * m_viewport[1]);
        auto endX   = startX + static_cast<int>(extent.width * m_viewport[2]);
        auto endY   = startY + static_cast<int>(extent.height * m_viewport[3]);

        if (startX <= position[0] && startY <= position[1] && position[0] <= endX && position[1] <= endY)
        {
            return true;
        }
    }

    return false;
}
