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
    actor->SetDevice(m_device);
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
