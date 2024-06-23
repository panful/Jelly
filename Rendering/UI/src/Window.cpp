#include "Window.h"
#include "Device.h"
#include <array>

using namespace Jelly;

Window::Window() noexcept
    : m_device(std::make_shared<Device>())
{
}

void Window::AddRenderer(std::shared_ptr<Renderer> renderer)
{
    m_renderers.emplace_back(std::move(renderer));
}

void Window::SetSize(const uint32_t width, const uint32_t height) noexcept
{
    m_width  = width;
    m_height = height;
}

void Window::SetTitle(const std::string_view title) noexcept
{
    m_title = title;
}

void Window::InitWindow() noexcept
{
    InitSwapChain();
    InitRenderPass();
}

void Window::InitSwapChain() noexcept
{
    m_swapChainData = SwapChainData(
        m_device, m_surface, vk::Extent2D {m_width, m_height}, nullptr, vk::ImageUsageFlagBits::eColorAttachment
    );
}

void Window::InitRenderPass() noexcept
{
    vk::AttachmentReference colorAttachment(0, vk::ImageLayout::eColorAttachmentOptimal);

    std::array colorAttachments {colorAttachment};

    vk::SubpassDescription subpassDescription(
        vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorAttachments, {}, {}, {}
    );

    std::array attachmentDescriptions {
        vk::AttachmentDescription {
                                   {},
                                   m_swapChainData.GetColorFormat(),
                                   vk::SampleCountFlagBits::e1,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eStore,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   vk::ImageLayout::ePresentSrcKHR
        }
    };

    vk::RenderPassCreateInfo renderPassCreateInfo(
        vk::RenderPassCreateFlags(), attachmentDescriptions, subpassDescription
    );

    m_renderPass = vk::raii::RenderPass(m_device->GetDevice(), renderPassCreateInfo);
}
