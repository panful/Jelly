#include "Viewer.h"
#include "Device.h"
#include "Renderer.h"

using namespace Jelly;

void Viewer::Init(std::shared_ptr<Device> device, uint32_t numberOfImages, const vk::Extent2D& extent)
{
    m_device         = std::move(device);
    m_numberOfImages = numberOfImages;
    m_extent         = extent;

    m_colorImageDatas.reserve(numberOfImages);
    for (uint32_t i = 0; i < numberOfImages; ++i)
    {
        m_colorImageDatas.emplace_back(ImageData(
            m_device,
            m_colorFormat,
            extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor
        ));
    }

    m_depthImageData = DepthImageData(m_device, m_depthFormat, extent);

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

    //------------------------------
    m_framebuffers.reserve(numberOfImages);
    for (uint32_t i = 0; i < numberOfImages; ++i)
    {
        std::array<vk::ImageView, 2> imageViews {m_colorImageDatas[i].GetImageView(), m_depthImageData.GetImageView()};

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, extent.width, extent.height, 1)
        ));
    }
}

void Viewer::Render(const vk::raii::CommandBuffer& commandBuffer)
{
    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue(0.1f, 0.2f, 0.3f, 1.f);
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.f, 0);
    vk::RenderPassBeginInfo renderPassBeginInfo(
        m_renderPass, m_framebuffers[m_currentImageIndex], vk::Rect2D({0, 0}, m_extent), clearValues
    );
    commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

    for (const auto& renderer : m_renderers)
    {
        renderer->Render(commandBuffer, m_renderPass);
    }

    commandBuffer.endRenderPass();

    m_currentImageIndex = (m_currentImageIndex + 1) % m_numberOfImages;
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

void Viewer::AddRenderer(std::shared_ptr<Renderer> renderer)
{
    m_renderers.emplace_back(std::move(renderer));
}
