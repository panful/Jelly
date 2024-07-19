#include "MsaaRenderPass.h"
#include "Device.h"
#include <array>

using namespace Jelly;

MsaaRenderPass::MsaaRenderPass(
    std::shared_ptr<Device> device, const vk::Extent2D& extent, vk::SampleCountFlagBits sampleCountFlagBits
)
    : RenderPass(std::move(device))
{
    m_sampleCountFlagBits = sampleCountFlagBits;

    m_colorImageDatas.resize(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        m_colorImageDatas[i] = std::make_unique<ImageData>(
            m_device,
            m_colorFormat,
            extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor,
            vk::SampleCountFlagBits::e1
        );
    }

    m_msaaColorImage = std::make_unique<ImageData>(
        m_device,
        m_colorFormat,
        extent,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment,
        vk::ImageLayout::eUndefined,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        vk::ImageAspectFlagBits::eColor,
        m_sampleCountFlagBits
    );

    m_depthImageData = std::make_unique<DepthImageData>(m_device, m_depthFormat, m_sampleCountFlagBits, extent);

    vk::AttachmentReference depthAttachment(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    std::array<vk::AttachmentReference, 0> inputAttachments;

    std::array<vk::AttachmentReference, 1> colorAttachments {
        vk::AttachmentReference {0, vk::ImageLayout::eColorAttachmentOptimal}
    };

    std::array<vk::AttachmentReference, 1> resolveAttachments {
        vk::AttachmentReference {2, vk::ImageLayout::eColorAttachmentOptimal}
    };

    vk::SubpassDescription subpassDescription(
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        inputAttachments,
        colorAttachments,
        resolveAttachments,
        &depthAttachment,
        {}
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
                                   vk::ImageLayout::eColorAttachmentOptimal
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
        },
        vk::AttachmentDescription {
                                   {},
                                   m_colorFormat, vk::SampleCountFlagBits::e1,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eStore,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   vk::ImageLayout::eShaderReadOnlyOptimal
        }
    };

    vk::RenderPassCreateInfo renderPassCreateInfo(
        vk::RenderPassCreateFlags(), attachmentDescriptions, subpassDescription
    );

    m_renderPass = vk::raii::RenderPass(m_device->GetDevice(), renderPassCreateInfo);

    m_framebuffers.reserve(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        std::array<vk::ImageView, 3> imageViews {
            m_msaaColorImage->GetImageView(), m_depthImageData->GetImageView(), m_colorImageDatas[i]->GetImageView()
        };

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, extent.width, extent.height, 1)
        ));
    }
}

void MsaaRenderPass::Resize(const vk::Extent2D& extent) noexcept
{
    m_colorImageDatas.resize(m_maximumOfFrames);
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        m_colorImageDatas[i] = std::make_unique<ImageData>(
            m_device,
            m_colorFormat,
            extent,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            vk::ImageLayout::eUndefined,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eColor,
            vk::SampleCountFlagBits::e1
        );
    }

    m_msaaColorImage = std::make_unique<ImageData>(
        m_device,
        m_colorFormat,
        extent,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment,
        vk::ImageLayout::eUndefined,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        vk::ImageAspectFlagBits::eColor,
        m_sampleCountFlagBits
    );

    m_depthImageData = std::make_unique<DepthImageData>(m_device, m_depthFormat, m_sampleCountFlagBits, extent);

    m_framebuffers.clear();
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        std::array<vk::ImageView, 3> imageViews {
            m_msaaColorImage->GetImageView(), m_depthImageData->GetImageView(), m_colorImageDatas[i]->GetImageView()
        };

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, extent.width, extent.height, 1)
        ));
    }
}

std::vector<vk::ClearValue> MsaaRenderPass::GetClearValues() const noexcept
{
    return {
        {vk::ClearColorValue(0.f, 0.f, 0.f, 0.f)},
        {vk::ClearDepthStencilValue(1.f, 0)},
        {vk::ClearColorValue(0.f, 0.f, 0.f, 0.f)}
    };
}
