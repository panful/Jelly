#include "Viewer.h"
#include "Device.h"
#include "Renderer.h"

using namespace Jelly;

void Viewer::SetDevice(std::shared_ptr<Device> device)
{
    m_device = std::move(device);
}

void Viewer::Init(const vk::Extent2D& extent)
{
    m_extent = extent;
    m_colorImageDatas.reserve(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        m_colorImageDatas.emplace_back(ImageData(
            m_device,
            m_colorFormat,
            m_extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor
        ));
    }

    m_depthImageData = std::make_unique<DepthImageData>(m_device, m_depthFormat, m_extent);

    vk::AttachmentReference colorAttachment(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::AttachmentReference depthAttachment(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
    std::array colorAttachments {colorAttachment};
    vk::SubpassDescription subpassDescription(
        vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorAttachments, {}, &depthAttachment, {}
    );
    std::array attachmentDescriptions {
        vk::AttachmentDescription {
                                   {},
                                   m_colorFormat, vk::SampleCountFlagBits::e1,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eStore,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   vk::ImageLayout::eShaderReadOnlyOptimal
        },
        vk::AttachmentDescription {
                                   {},
                                   m_depthFormat, vk::SampleCountFlagBits::e1,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   vk::ImageLayout::eDepthStencilAttachmentOptimal
        }
    };

    vk::RenderPassCreateInfo renderPassCreateInfo(
        vk::RenderPassCreateFlags(), attachmentDescriptions, subpassDescription
    );
    m_renderPass = vk::raii::RenderPass(m_device->GetDevice(), renderPassCreateInfo);

    m_framebuffers.reserve(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        std::array<vk::ImageView, 2> imageViews {m_colorImageDatas[i].GetImageView(), m_depthImageData->GetImageView()};

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_extent.width, m_extent.height, 1)
        ));
    }
}

void Viewer::Resize(const vk::Extent2D& extent)
{
    m_extent            = extent;
    m_currentFrameIndex = 0;
    m_device->GetDevice().waitIdle();

    m_colorImageDatas.clear();
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        m_colorImageDatas.emplace_back(ImageData(
            m_device,
            m_colorFormat,
            m_extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor
        ));
    }

    m_depthImageData = std::make_unique<DepthImageData>(m_device, m_depthFormat, m_extent);

    m_framebuffers.clear();
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        std::array<vk::ImageView, 2> imageViews {m_colorImageDatas[i].GetImageView(), m_depthImageData->GetImageView()};

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_extent.width, m_extent.height, 1)
        ));
    }
}

void Viewer::Render(const vk::raii::CommandBuffer& commandBuffer)
{
    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue(0.f, 0.f, 0.f, 0.f);
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.f, 0);
    vk::RenderPassBeginInfo renderPassBeginInfo(
        m_renderPass, m_framebuffers[m_currentFrameIndex], vk::Rect2D({0, 0}, m_extent), clearValues
    );
    commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

    for (const auto& renderer : m_renderers)
    {
        renderer->Render(commandBuffer);
    }

    commandBuffer.endRenderPass();

    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_maximumOfFrames;
}

const std::vector<std::shared_ptr<Renderer>>& Viewer::GetAllRenderers() const noexcept
{
    return m_renderers;
}

std::vector<vk::ImageView> Viewer::GetColorImageViews() const noexcept
{
    std::vector<vk::ImageView> imageViews {};
    for (const auto& colorImageData : m_colorImageDatas)
    {
        imageViews.emplace_back(colorImageData.GetImageView());
    }
    return imageViews;
}

uint32_t Viewer::GetCurrentFrameIndex() const noexcept
{
    return m_currentFrameIndex;
}

uint32_t Viewer::GetMaximumOfFrames() const noexcept
{
    return m_maximumOfFrames;
}

const vk::Extent2D& Viewer::GetExtent() const noexcept
{
    return m_extent;
}

const vk::raii::RenderPass& Viewer::GetRenderPass() const noexcept
{
    return m_renderPass;
}

void Viewer::AddRenderer(std::shared_ptr<Renderer> renderer)
{
    renderer->SetDevice(m_device);
    renderer->SetViewer(weak_from_this());
    m_renderers.emplace_back(std::move(renderer));
}
