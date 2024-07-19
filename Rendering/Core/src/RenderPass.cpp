#include "RenderPass.h"
#include "Device.h"
#include <array>

using namespace Jelly;

RenderPass::RenderPass(std::shared_ptr<Device> device, const vk::Extent2D& extent)
    : m_device(std::move(device))
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

    m_depthImageData = std::make_unique<DepthImageData>(m_device, m_depthFormat, vk::SampleCountFlagBits::e1, extent);

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
        std::array<vk::ImageView, 2> imageViews {
            m_colorImageDatas[i]->GetImageView(), m_depthImageData->GetImageView()
        };

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, extent.width, extent.height, 1)
        ));
    }
}

void RenderPass::Resize(const vk::Extent2D& extent) noexcept
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

    m_depthImageData = std::make_unique<DepthImageData>(m_device, m_depthFormat, vk::SampleCountFlagBits::e1, extent);

    m_framebuffers.clear();
    for (uint32_t i = 0; i < m_maximumOfFrames; ++i)
    {
        std::array<vk::ImageView, 2> imageViews {
            m_colorImageDatas[i]->GetImageView(), m_depthImageData->GetImageView()
        };

        m_framebuffers.emplace_back(vk::raii::Framebuffer(
            m_device->GetDevice(),
            vk::FramebufferCreateInfo({}, m_renderPass, imageViews, extent.width, extent.height, 1)
        ));
    }
}

const vk::raii::RenderPass& RenderPass::GetRenderPass() const noexcept
{
    return m_renderPass;
}

const std::vector<vk::raii::Framebuffer>& RenderPass::GetFramebuffers() const noexcept
{
    return m_framebuffers;
}

std::vector<vk::ImageView> RenderPass::GetColorImageViews() const noexcept
{
    std::vector<vk::ImageView> imageViews {};
    for (const auto& colorImageData : m_colorImageDatas)
    {
        imageViews.emplace_back(colorImageData->GetImageView());
    }
    return imageViews;
}
