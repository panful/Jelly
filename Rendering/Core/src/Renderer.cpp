#include "Renderer.h"
#include "Actor.h"
#include "Logger.h"
#include "Window.h"

using namespace Jelly;

void Renderer::Render(const vk::raii::CommandBuffer& commandBuffer) noexcept
{
    Logger::GetInstance()->Trace();

    auto window = m_window.lock(); // 如果 Renderer 存在，那么 Window 一定存在，此处不用判空

    auto offset = vk::Offset2D {
        static_cast<int32_t>(window->GetSize().width * m_viewport[0]),
        static_cast<int32_t>(window->GetSize().height * m_viewport[1])
    };
    auto extent = vk::Extent2D {
        static_cast<uint32_t>(window->GetSize().width * m_viewport[2]),
        static_cast<uint32_t>(window->GetSize().height * m_viewport[3])
    };
    auto m_viewport = vk::Viewport {
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

    commandBuffer.setViewport(0, m_viewport);
    commandBuffer.setScissor(0, vk::Rect2D(offset, extent));
    commandBuffer.clearAttachments(attachment, rect);

    for (auto& actor : m_actors)
    {
        actor->Render();
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
}

void Renderer::SetWindow(std::shared_ptr<Window> window) noexcept
{
    m_window = window;
}

void Renderer::SetViewport(const std::array<double, 4>& viewport)
{
    m_viewport = viewport;
}

void Renderer::SetBackground(const std::array<float, 4>& background)
{
    m_background = background;
}
