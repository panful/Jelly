#include "PickRenderPass.h"
#include "Device.h"
#include <array>

using namespace Jelly;

PickRenderPass::PickRenderPass(const vk::Extent2D& extent)
    : RenderPass(extent)
{
    m_maximumOfFrames = 1; // 拾取时只需要绘制一帧

    m_colorImageDatas.resize(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        m_colorImageDatas[i] = std::make_unique<ImageData>(
            m_colorFormat,
            m_extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor,
            m_sampleCountFlagBits
        );
    }

    m_depthImageData = std::make_unique<DepthImageData>(m_depthFormat, m_sampleCountFlagBits, m_extent);

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
                                   vk::ImageLayout::eTransferSrcOptimal
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

    m_renderPass = vk::raii::RenderPass(Device::Get()->GetDevice(), renderPassCreateInfo);

    m_framebuffers.reserve(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        std::array<vk::ImageView, 2> imageViews {
            m_colorImageDatas[i]->GetImageView(), m_depthImageData->GetImageView()
        };

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            Device::Get()->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_extent.width, m_extent.height, 1)
        ));
    }
}

void PickRenderPass::Resize(const vk::Extent2D& extent) noexcept
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
            m_colorFormat,
            m_extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor,
            m_sampleCountFlagBits
        );
    }

    m_depthImageData = std::make_unique<DepthImageData>(m_depthFormat, m_sampleCountFlagBits, m_extent);

    m_framebuffers.clear();
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        std::array<vk::ImageView, 2> imageViews {
            m_colorImageDatas[i]->GetImageView(), m_depthImageData->GetImageView()
        };

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            Device::Get()->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, m_extent.width, m_extent.height, 1)
        ));
    }
}

std::vector<vk::ClearValue> PickRenderPass::GetClearValues() const noexcept
{
    return {{vk::ClearColorValue(0.f, 0.f, 0.f, 0.f)}, {vk::ClearDepthStencilValue(1.f, 0)}};
}

const std::unique_ptr<ImageData>& PickRenderPass::GetColorImageData() const noexcept
{
    return m_colorImageDatas[0];
}
