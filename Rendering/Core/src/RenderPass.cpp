#include "RenderPass.h"

using namespace Jelly;

RenderPass::RenderPass(const vk::Extent2D& extent)
    : m_extent(extent)
{
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

vk::SampleCountFlagBits RenderPass::GetSampleCountFlagBits() const noexcept
{
    return m_sampleCountFlagBits;
}

const vk::Extent2D& RenderPass::GetExtent() const noexcept
{
    return m_extent;
}
