#include "Renderer.h"
#include "Actor.h"
#include "Logger.h"
#include "Viewer.h"
#include "Window.h"

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

        if (static_cast<int>(extent.width * m_viewport[0]) <= position[0]
            && static_cast<int>(extent.width * m_viewport[1]) <= position[1]
            && static_cast<int>(extent.width * m_viewport[2]) >= position[0]
            && static_cast<int>(extent.width * m_viewport[3]) >= position[1])
        {
            return true;
        }
    }

    return false;
}
