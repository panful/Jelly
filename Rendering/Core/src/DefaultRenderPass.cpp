#include "DefaultRenderPass.h"
#include "Device.h"
#include <array>

using namespace Jelly;

DefaultRenderPass::DefaultRenderPass(std::shared_ptr<Device> device, const vk::Extent2D& extent)
    : RenderPass(std::move(device), extent)
{
    m_colorImageDatas.resize(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        m_colorImageDatas[i] = std::make_unique<ImageData>(
            m_device,
            m_colorFormat,
            m_extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor,
            m_sampleCountFlagBits
        );
    }

    m_depthImageData = std::make_unique<DepthImageData>(m_device, m_depthFormat, m_sampleCountFlagBits, extent);

    vk::AttachmentReference colorAttachment(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::AttachmentReference depthAttachment(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    std::array colorAttachments {colorAttachment};

    vk::SubpassDescription subpassDescription(
        vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorAttachments, {}, &depthAttachment, {}
    );

    std::array attachmentDescriptions {
        vk::AttachmentDescription {
                                   {},
                                   m_colorFormat, m_sampleCountFlagBits,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eStore,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   vk::ImageLayout::eShaderReadOnlyOptimal
        },
        vk::AttachmentDescription {
                                   {},
                                   m_depthFormat, m_sampleCountFlagBits,
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
        std::array<vk::ImageView, 2> imageViews {
            m_colorImageDatas[i]->GetImageView(), m_depthImageData->GetImageView()
        };

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_extent.width, m_extent.height, 1)
        ));
    }
}

void DefaultRenderPass::Resize(const vk::Extent2D& extent) noexcept
{
    if (extent == m_extent)
    {
        return;
    }

    m_extent = extent;
    m_colorImageDatas.resize(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        m_colorImageDatas[i] = std::make_unique<ImageData>(
            m_device,
            m_colorFormat,
            m_extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor,
            m_sampleCountFlagBits
        );
    }

    m_depthImageData = std::make_unique<DepthImageData>(m_device, m_depthFormat, m_sampleCountFlagBits, m_extent);

    m_framebuffers.clear();
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        std::array<vk::ImageView, 2> imageViews {
            m_colorImageDatas[i]->GetImageView(), m_depthImageData->GetImageView()
        };

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_extent.width, m_extent.height, 1)
        ));
    }
}

std::vector<vk::ClearValue> DefaultRenderPass::GetClearValues() const noexcept
{
    return {{vk::ClearColorValue(0.f, 0.f, 0.f, 0.f)}, {vk::ClearDepthStencilValue(1.f, 0)}};
}
