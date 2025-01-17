#include "Window.h"
#include "Device.h"
#include "ImageData.h"
#include "Logger.h"
#include "MemoryHelper.h"
#include "Renderer.h"
#include "SpvCreater.h"
#include "Viewer.h"
#include <array>
#include <limits>
#include <mutex>

using namespace Jelly;

Window::Window() noexcept
    : m_viewer(std::make_shared<Viewer>())
{
}

Window::~Window() noexcept
{
    Device::Get()->GetDevice().waitIdle();
    std::cout << __func__ << std::endl;
}

void Window::AddRenderer(std::shared_ptr<Renderer> renderer)
{
    m_viewer->AddRenderer(std::move(renderer));
}

void Window::SetSize(const uint32_t width, const uint32_t height) noexcept
{
    if ((width == m_extent.width && height == m_extent.height) || width == 0 || height == 0)
    {
        return;
    }

    m_extent = vk::Extent2D {width, height};

    if (m_initialized)
    {
        Device::Get()->GetDevice().waitIdle();
        m_viewer->Resize(vk::Extent2D {width, height});

        m_currentFrameIndex = 0;
        RecreateSwapChain();
        UpdateDescriptorSets();
    }
}

void Window::SetTitle(const std::string_view title) noexcept
{
    m_title = title;
}

void Window::SetEnableGetRenderingResult(bool enable) noexcept
{
    m_enableGetRenderingResult = enable;
}

void Window::SetSampleCount(vk::SampleCountFlagBits sampleCount) noexcept
{
    m_viewer->SetSampleCount(sampleCount);
}

const std::vector<std::shared_ptr<Renderer>>& Window::GetAllRenderers() const noexcept
{
    return m_viewer->GetAllRenderers();
}

const vk::Extent2D& Window::GetSize() const noexcept
{
    return m_extent;
}

const std::any& Window::GetNativeWindow() const noexcept
{
    return m_window;
}

std::vector<uint8_t> Window::GetLastRenderingResult() const noexcept
{
    std::vector<uint8_t> retVal {};
    if (!m_enableGetRenderingResult || vk::Format::eB8G8R8A8Unorm != m_swapChainData->GetColorFormat())
    {
        // 暂时只支持获取图像格式为 eB8G8R8A8Unorm 的交换链图像
        return retVal;
    }

    ImageData imageData(
        m_swapChainData->GetColorFormat(),
        m_extent,
        vk::ImageTiling::eLinear,
        vk::ImageUsageFlagBits::eTransferDst,
        vk::ImageLayout::eUndefined,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        vk::ImageAspectFlagBits::eColor,
        vk::SampleCountFlagBits::e1,
        false
    );

    MemoryHelper::OneTimeSubmit([&imageData, this](const vk::raii::CommandBuffer& cmd) {
        auto&& format   = this->m_swapChainData->GetColorFormat();
        auto&& inImage  = this->m_swapChainData->GetImage(this->m_currentImageIndex);
        auto&& outImage = imageData.GetImage();

        ImageData::SetImageLayout(
            cmd, outImage, format, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal
        );

        ImageData::SetImageLayout(
            cmd, inImage, format, vk::ImageLayout::ePresentSrcKHR, vk::ImageLayout::eTransferSrcOptimal
        );

        auto formatProperties = Device::Get()->GetPhysicalDevice().getFormatProperties(format);

        if ((formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc)
            && (formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eBlitDst))
        {
            vk::ImageSubresourceLayers imageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
            std::array<vk::Offset3D, 2> offsets {
                vk::Offset3D(0, 0, 0),
                vk::Offset3D(static_cast<int32_t>(this->m_extent.width), static_cast<int32_t>(this->m_extent.height), 1)
            };
            vk::ImageBlit imageBlit(imageSubresourceLayers, offsets, imageSubresourceLayers, offsets);
            cmd.blitImage(
                inImage,
                vk::ImageLayout::eTransferSrcOptimal,
                outImage,
                vk::ImageLayout::eTransferDstOptimal,
                imageBlit,
                vk::Filter::eLinear
            );
        }
        else
        {
            vk::ImageSubresourceLayers imageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
            vk::ImageCopy imageCopy(
                imageSubresourceLayers,
                vk::Offset3D(),
                imageSubresourceLayers,
                vk::Offset3D(),
                vk::Extent3D(this->m_extent, 1)
            );
            cmd.copyImage(
                inImage, vk::ImageLayout::eTransferSrcOptimal, outImage, vk::ImageLayout::eTransferDstOptimal, imageCopy
            );
        }

        ImageData::SetImageLayout(
            cmd, outImage, format, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eGeneral
        );

        ImageData::SetImageLayout(
            cmd, inImage, format, vk::ImageLayout::eTransferSrcOptimal, vk::ImageLayout::ePresentSrcKHR
        );
    });

    auto subResourceLayout = imageData.GetImage().getSubresourceLayout({vk::ImageAspectFlagBits::eColor, 0, 0});
    auto pixels            = reinterpret_cast<uint8_t*>(imageData.GetDeviceMemory().mapMemory(0, vk::WholeSize, {}));
    pixels += subResourceLayout.offset;

    retVal.reserve(m_extent.width * m_extent.height * 4);
    for (uint32_t y = 0; y < m_extent.height; y++)
    {
        auto row = pixels;
        for (uint32_t x = 0; x < m_extent.width; x++)
        {
            retVal.emplace_back(*(row + 2));
            retVal.emplace_back(*(row + 1));
            retVal.emplace_back(*(row + 0));
            retVal.emplace_back(*(row + 3));
            row += 4;
        }
        pixels += subResourceLayout.rowPitch;
    }

    imageData.GetDeviceMemory().unmapMemory();

    return retVal;
}

bool Window::HasInitialized() const noexcept
{
    return m_initialized;
}

void Window::Render() noexcept
{
    static std::once_flag flag {};
    std::call_once(flag, [this]() {
        this->InitWindow();
        this->m_initialized = true;
    });

    PreRender();
    m_viewer->Render(m_commandBuffers[m_currentFrameIndex]);
    Present();
    PostRender();
}

void Window::PreRender() noexcept
{
    auto fenceResult = Device::Get()->GetDevice().waitForFences(
        {m_inFlightFences[m_currentFrameIndex]}, vk::True, std::numeric_limits<uint64_t>::max()
    );

    if (vk::Result::eSuccess != fenceResult)
    {
        Logger::GetInstance()->Error("Failed to wait for fence");
    }

    vk::Result imageResult {};
    std::tie(imageResult, m_currentImageIndex) = m_swapChainData->GetSwapChain().acquireNextImage(
        std::numeric_limits<uint64_t>::max(), m_imageAcquiredSemaphores[m_currentFrameIndex]
    );

    Device::Get()->GetDevice().resetFences({m_inFlightFences[m_currentFrameIndex]});

    auto&& cmd = m_commandBuffers[m_currentFrameIndex];
    cmd.reset();
    cmd.begin({});
}

void Window::PostRender() noexcept
{
    auto&& cmd = m_commandBuffers[m_currentFrameIndex];
    cmd.end();

    std::array<vk::CommandBuffer, 1> drawCommandBuffers {cmd};
    std::array<vk::Semaphore, 1> signalSemaphores {m_renderFinishedSemaphores[m_currentFrameIndex]};
    std::array<vk::Semaphore, 1> waitSemaphores {m_imageAcquiredSemaphores[m_currentFrameIndex]};
    std::array<vk::PipelineStageFlags, 1> waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::SubmitInfo drawSubmitInfo(waitSemaphores, waitStages, drawCommandBuffers, signalSemaphores);
    Device::Get()->GetGraphicsQueue().submit(drawSubmitInfo, m_inFlightFences[m_currentFrameIndex]);

    std::array<vk::Semaphore, 1> presentWait {m_renderFinishedSemaphores[m_currentFrameIndex]};
    std::array<vk::SwapchainKHR, 1> swapchains {m_swapChainData->GetSwapChain()};
    vk::PresentInfoKHR presentInfoKHR(presentWait, swapchains, m_currentImageIndex);

    auto presentResult = Device::Get()->GetPresentQueue().presentKHR(presentInfoKHR);

    if (vk::Result::eSuccess != presentResult)
    {
        Logger::GetInstance()->Error("Failed to present");
    }

    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_numberOfFrames;
}

void Window::Present() const noexcept
{
    std::array<vk::ClearValue, 1> clearValues {};
    clearValues[0].color = vk::ClearColorValue(0.f, 0.f, 0.f, 0.f);

    vk::RenderPassBeginInfo renderPassBeginInfo(
        m_renderPass, m_framebuffers[m_currentFrameIndex], vk::Rect2D(vk::Offset2D(0, 0), m_extent), clearValues
    );

    auto&& cmd = m_commandBuffers[m_currentFrameIndex];
    cmd.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline->GetPipeline());
    cmd.setViewport(
        0, vk::Viewport(0.0f, 0.0f, static_cast<float>(m_extent.width), static_cast<float>(m_extent.height), 0.f, 1.f)
    );
    cmd.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_extent));
    cmd.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        m_pipeline->GetPipelineLayout(),
        0,
        {m_descriptorSets[m_currentFrameIndex]},
        nullptr
    );
    cmd.draw(3, 1, 0, 0);
    cmd.endRenderPass();
}

void Window::InitWindow() noexcept
{
    InitSurface();

    Device::Get()->PickPhysicalDevice(m_surface);
    Device::Get()->InitDevice();
    Device::Get()->InitQueues();
    Device::Get()->InitCommandPool();
    Device::Get()->InitDescriptorPool();
    Device::Get()->InitPipelineCache();

    InitSwapChain();
    InitViewer();
    InitRenderPass();
    InitPipeline();
    InitFramebuffers();
    InitSyncObjects();
    InitCommandBuffers();
    InitSampler();
    InitDescriptorSets();
    UpdateDescriptorSets();
}

void Window::InitSwapChain() noexcept
{
    auto usage = m_enableGetRenderingResult
        ? vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc
        : vk::ImageUsageFlagBits::eColorAttachment;

    m_swapChainData = std::make_unique<SwapChainData>(m_surface, m_extent, nullptr, usage);

    // 创建的窗口大小可能不等于期望的大小
    m_extent = vk::Extent2D {m_swapChainData->GetExtent().width, m_swapChainData->GetExtent().height};
}

void Window::RecreateSwapChain() noexcept
{
    auto usage = m_enableGetRenderingResult
        ? vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc
        : vk::ImageUsageFlagBits::eColorAttachment;

    m_swapChainData = std::make_unique<SwapChainData>(m_surface, m_extent, &m_swapChainData->GetSwapChain(), usage);

    const auto numberOfImages = m_swapChainData->GetNumberOfImages();
    m_framebuffers.clear();
    for (uint32_t i = 0; i < numberOfImages; ++i)
    {
        std::array<vk::ImageView, 1> imageViews {m_swapChainData->GetImageView(i)};
        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            Device::Get()->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_extent.width, m_extent.height, 1)
        ));
    }
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
                                   m_swapChainData->GetColorFormat(),
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

    m_renderPass = vk::raii::RenderPass(Device::Get()->GetDevice(), renderPassCreateInfo);
}

void Window::InitFramebuffers() noexcept
{
    const auto numberOfImages = m_swapChainData->GetNumberOfImages();
    m_framebuffers.reserve(numberOfImages);
    for (uint32_t i = 0; i < numberOfImages; ++i)
    {
        std::array<vk::ImageView, 1> imageViews {m_swapChainData->GetImageView(i)};
        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            Device::Get()->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_extent.width, m_extent.height, 1)
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
        m_inFlightFences.emplace_back(vk::raii::Fence(Device::Get()->GetDevice(), {vk::FenceCreateFlagBits::eSignaled})
        );
        m_renderFinishedSemaphores.emplace_back(
            vk::raii::Semaphore(Device::Get()->GetDevice(), vk::SemaphoreCreateInfo())
        );
        m_imageAcquiredSemaphores.emplace_back(
            vk::raii::Semaphore(Device::Get()->GetDevice(), vk::SemaphoreCreateInfo())
        );
    }
}

void Window::InitCommandBuffers() noexcept
{
    m_commandBuffers = vk::raii::CommandBuffers(
        Device::Get()->GetDevice(),
        vk::CommandBufferAllocateInfo {
            Device::Get()->GetCommandPool(), vk::CommandBufferLevel::ePrimary, m_numberOfFrames
        }
    );
}

void Window::InitSampler() noexcept
{
    m_sampler = vk::raii::Sampler(
        Device::Get()->GetDevice(),
        vk::SamplerCreateInfo {
            {},
            vk::Filter::eLinear,
            vk::Filter::eLinear,
            vk::SamplerMipmapMode::eLinear,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            0.0f,
            false,
            16.0f,
            false,
            vk::CompareOp::eNever,
            0.0f,
            0.0f,
            vk::BorderColor::eFloatOpaqueBlack
        }
    );
}

void Window::InitPipeline() noexcept
{
    static std::string_view vertCode = R"(
        #version 450
        layout (location = 0) out vec2 outUV;
        void main() {
            outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
            gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
        }
    )";
    static std::string_view fragCode = R"(
        #version 450
        layout (location = 0) in vec2 inUV;
        layout (location = 0) out vec4 outFragColor;
        layout (binding = 0) uniform sampler2D samplerColor;
        void main() {
            outFragColor = texture(samplerColor, inUV);
        }
    )";

    auto vertSpv = SpvCreater::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eVertex, vertCode);
    auto fragSpv = SpvCreater::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eFragment, fragCode);

    PipelineInfo pipelineInfo {
        .vertexShaderCode   = std::move(vertSpv.value()),
        .fragmentShaderCode = std::move(fragSpv.value()),
        .strides            = {},
        .descriptorSetLayoutBindings =
            {{0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment}},
        .renderPass = m_renderPass
    };
    m_pipeline = std::make_unique<Pipeline>(pipelineInfo);
}

void Window::InitViewer() noexcept
{
    m_viewer->Init(m_extent);
}

void Window::InitDescriptorSets() noexcept
{
    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(m_numberOfFrames, m_pipeline->GetDescriptorSetLayout());

    m_descriptorSets = vk::raii::DescriptorSets(
        Device::Get()->GetDevice(),
        vk::DescriptorSetAllocateInfo {Device::Get()->GetDescriptorPool(), descriptorSetLayouts}
    );
}

void Window::UpdateDescriptorSets() noexcept
{
    auto&& imageViews = m_viewer->GetColorImageViews();

    for (uint32_t i = 0; i < m_numberOfFrames; ++i)
    {
        vk::DescriptorImageInfo descriptorImageInfo(m_sampler, imageViews[i], vk::ImageLayout::eShaderReadOnlyOptimal);
        std::array<vk::WriteDescriptorSet, 1> writeDescriptorSets {
            {{m_descriptorSets[i], 0, 0, vk::DescriptorType::eCombinedImageSampler, descriptorImageInfo, nullptr}}
        };
        Device::Get()->GetDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
    }
}
