#include "Window.h"
#include "Device.h"
#include "Logger.h"
#include <array>
#include <limits>

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

void Window::PreRender() noexcept
{
    auto fenceResult = m_device->GetDevice().waitForFences(
        {m_inFlightFences[m_currentFrameIndex]}, vk::True, std::numeric_limits<uint64_t>::max()
    );

    if (vk::Result::eSuccess != fenceResult)
    {
        Logger::GetInstance()->Error("Failed to wait for fence");
    }

    vk::Result imageResult {};
    std::tie(imageResult, m_currentImageIndex) = m_swapChainData.GetSwapChain().acquireNextImage(
        std::numeric_limits<uint64_t>::max(), m_imageAcquiredSemaphores[m_currentFrameIndex]
    );

    auto&& cmd = m_commandBuffers[m_currentFrameIndex];
    cmd.reset();

    std::array<vk::ClearValue, 1> clearValues {};
    clearValues[0].color = vk::ClearColorValue(.1f, .2f, .3f, 1.f);

    vk::RenderPassBeginInfo renderPassBeginInfo(
        *m_renderPass,
        m_framebuffers[m_currentFrameIndex],
        vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(m_width, m_height)),
        clearValues
    );

    cmd.begin({});
    cmd.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
}

void Window::PostRender() noexcept
{
    auto&& cmd = m_commandBuffers[m_currentFrameIndex];
    cmd.endRenderPass();
    cmd.end();

    std::array<vk::CommandBuffer, 1> drawCommandBuffers {cmd};
    std::array<vk::Semaphore, 1> signalSemaphores {m_renderFinishedSemaphores[m_currentFrameIndex]};
    std::array<vk::Semaphore, 1> waitSemaphores {m_imageAcquiredSemaphores[m_currentFrameIndex]};
    std::array<vk::PipelineStageFlags, 1> waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::SubmitInfo drawSubmitInfo(waitSemaphores, waitStages, drawCommandBuffers, signalSemaphores);
    m_device->GetGraphicsQueue().submit(drawSubmitInfo, m_inFlightFences[m_currentFrameIndex]);

    std::array<vk::Semaphore, 1> presentWait {m_renderFinishedSemaphores[m_currentFrameIndex]};
    std::array<vk::SwapchainKHR, 1> swapchains {m_swapChainData.GetSwapChain()};
    vk::PresentInfoKHR presentInfoKHR(presentWait, swapchains, m_currentImageIndex);

    auto presentResult = m_device->GetPresentQueue().presentKHR(presentInfoKHR);

    if (vk::Result::eSuccess != presentResult)
    {
        Logger::GetInstance()->Error("Failed to present");
    }

    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_numberOfFrames;
}

void Window::InitWindow() noexcept
{
    InitSwapChain();
    InitRenderPass();
    InitFramebuffers();
    InitSyncObjects();
    InitCommandPool();
    InitCommandBuffers();
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

void Window::InitCommandPool() noexcept
{
    m_commandPool = vk::raii::CommandPool(
        m_device->GetDevice(), {{vk::CommandPoolCreateFlagBits::eResetCommandBuffer}, m_device->GetGraphicsQueueIndex()}
    );
}

void Window::InitCommandBuffers() noexcept
{
    m_commandBuffers = vk::raii::CommandBuffers(
        m_device->GetDevice(),
        vk::CommandBufferAllocateInfo {m_commandPool, vk::CommandBufferLevel::ePrimary, m_numberOfFrames}
    );
}
