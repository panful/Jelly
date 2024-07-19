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
    m_extent     = extent;
    m_renderPass = std::make_unique<RenderPass>(m_device, m_extent);
}

void Viewer::Resize(const vk::Extent2D& extent)
{
    m_extent            = extent;
    m_currentFrameIndex = 0;
    m_device->GetDevice().waitIdle();
    m_renderPass->Resize(m_extent);
}

void Viewer::Render(const vk::raii::CommandBuffer& commandBuffer)
{
    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue(0.f, 0.f, 0.f, 0.f);
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.f, 0);
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
    renderer->SetDevice(m_device);
    renderer->SetViewer(weak_from_this());
    m_renderers.emplace_back(std::move(renderer));
}
