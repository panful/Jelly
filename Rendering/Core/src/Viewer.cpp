#include "Viewer.h"
#include "DefaultRenderPass.h"
#include "Device.h"
#include "MsaaRenderPass.h"
#include "Renderer.h"

using namespace Jelly;

void Viewer::SetSampleCount(vk::SampleCountFlagBits sampleCount) noexcept
{
    m_sampleCountFlagBits = sampleCount;
}

void Viewer::Init(const vk::Extent2D& extent)
{
    m_extent = extent;

    if (m_sampleCountFlagBits != vk::SampleCountFlagBits::e1)
    {
        auto properties = Device::Get()->GetPhysicalDevice().getProperties();

        vk::SampleCountFlags counts =
            properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;

        m_sampleCountFlagBits = counts & vk::SampleCountFlagBits::e64 ? vk::SampleCountFlagBits::e64
            : counts & vk::SampleCountFlagBits::e32                   ? vk::SampleCountFlagBits::e32
            : counts & vk::SampleCountFlagBits::e16                   ? vk::SampleCountFlagBits::e16
            : counts & vk::SampleCountFlagBits::e8                    ? vk::SampleCountFlagBits::e8
            : counts & vk::SampleCountFlagBits::e4                    ? vk::SampleCountFlagBits::e4
            : counts & vk::SampleCountFlagBits::e2                    ? vk::SampleCountFlagBits::e2
                                                                      : vk::SampleCountFlagBits::e1;
    }

    if (m_sampleCountFlagBits != vk::SampleCountFlagBits::e1)
    {
        m_renderPass = std::make_unique<MsaaRenderPass>(m_extent, m_sampleCountFlagBits);
    }
    else
    {
        m_renderPass = std::make_unique<DefaultRenderPass>(m_extent);
    }
}

void Viewer::Resize(const vk::Extent2D& extent)
{
    m_extent            = extent;
    m_currentFrameIndex = 0;
    Device::Get()->GetDevice().waitIdle();
    m_renderPass->Resize(m_extent);
}

void Viewer::Render(const vk::raii::CommandBuffer& commandBuffer)
{
    auto clearValues = m_renderPass->GetClearValues();

    vk::RenderPassBeginInfo renderPassBeginInfo(
        m_renderPass->GetRenderPass(),
        m_renderPass->GetFramebuffers()[m_currentFrameIndex],
        vk::Rect2D({0, 0}, m_extent),
        clearValues
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
    return m_renderPass->GetColorImageViews();
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

const std::unique_ptr<RenderPass>& Viewer::GetRenderPass() const noexcept
{
    return m_renderPass;
}

void Viewer::AddRenderer(std::shared_ptr<Renderer> renderer)
{
    renderer->SetViewer(weak_from_this());
    m_renderers.emplace_back(std::move(renderer));
}
