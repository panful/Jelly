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
    InitFramebuffers();
    InitSyncObjects();
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

void Window::InitFramebuffers() noexcept
{
    const auto numberOfImages = m_swapChainData.GetNumberOfImages();
    m_framebuffers.reserve(numberOfImages);
    for (uint32_t i = 0; i < numberOfImages; ++i)
    {
        std::array<vk::ImageView, 1> imageViews {m_swapChainData.GetImageView(i)};
        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(), vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_width, m_height, 1)
        ));
    }
}

void Window::InitSyncObjects() noexcept
{
    m_inFlightFences.reserve(m_numberOfFrames);
    m_renderFinishedSemaphores.reserve(m_numberOfFrames);
    m_imageAcquiredSemaphores.reserve(m_numberOfFrames);
    for (uint32_t i = 0; i < m_numberOfFrames; ++i)
    {
        m_inFlightFences.emplace_back(vk::raii::Fence(m_device->GetDevice(), {vk::FenceCreateFlagBits::eSignaled}));
        m_renderFinishedSemaphores.emplace_back(vk::raii::Semaphore(m_device->GetDevice(), vk::SemaphoreCreateInfo()));
        m_imageAcquiredSemaphores.emplace_back(vk::raii::Semaphore(m_device->GetDevice(), vk::SemaphoreCreateInfo()));
    }
}
