#include "Mapper.h"
#include "Device.h"
#include "Renderer.h"
#include "Viewer.h"
#include <functional>

using namespace Jelly;

void Mapper::Render(
    const vk::raii::CommandBuffer& commandBuffer, const std::shared_ptr<Viewer>& viewer, Renderer* renderer
) noexcept
{
    Configure(viewer);

    DeviceRender(commandBuffer, viewer, renderer);
}

void Mapper::DeviceRender(
    const vk::raii::CommandBuffer& commandBuffer, const std::shared_ptr<Viewer>& viewer, Renderer* renderer
)
{
    auto&& pipeline = m_device->GetPipelineCache()->GetPipeline(m_pipelineKey);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());
    if (m_useUniformColor)
    {
        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            pipeline->GetPipelineLayout(),
            0,
            {m_descriptorSets[viewer->GetCurrentFrameIndex()]},
            nullptr
        );
    }

    std::array<float, 16> model {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
    auto camera = renderer->GetCamera();
    std::vector<std::array<float, 16>> transform {model, camera->GetViewMatrix(), camera->GetProjectMatrix()};

    commandBuffer.pushConstants<std::array<float, 16>>(
        pipeline->GetPipelineLayout(), vk::ShaderStageFlagBits::eVertex, 0, std::move(transform)
    );
    commandBuffer.bindVertexBuffers(0, m_drawable->GetVertexBuffers(), m_drawable->GetVertexOffsets());
    commandBuffer.bindIndexBuffer(m_drawable->GetIndexBuffer(), 0, m_drawable->GetIndexType());
    commandBuffer.drawIndexed(m_drawable->GetIndexCount(), 1, 0, 0, 0);
}

void Mapper::SetDevice(std::shared_ptr<Device> device) noexcept
{
    m_device = std::move(device);
}

void Mapper::BuildPipeline(const std::shared_ptr<Viewer>& viewer, const PipelineInfo& pipelineInfo) noexcept
{
    m_pipelineKey = std::hash<PipelineInfo>()(pipelineInfo);

    if (!m_device->GetPipelineCache()->HasPipeline(m_pipelineKey))
    {
        m_device->GetPipelineCache()->AddPipeline(m_pipelineKey, std::make_unique<Pipeline>(m_device, pipelineInfo));
    }

    if (ColorMode::UniformColor == m_colorMode)
    {
        m_uniformBufferObjects.reserve(viewer->GetMaximumOfFrames());
        for (uint32_t i = 0; i < viewer->GetMaximumOfFrames(); ++i)
        {
            m_uniformBufferObjects.emplace_back(
                m_device,
                sizeof(m_color),
                vk::BufferUsageFlagBits::eUniformBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
            );
            MemoryHelper::CopyToDevice(
                m_uniformBufferObjects[i].GetDeviceMemory(), m_color.data(), m_color.size(), sizeof(float)
            );
        }

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(
            viewer->GetMaximumOfFrames(),
            m_device->GetPipelineCache()->GetPipeline(m_pipelineKey)->GetDescriptorSetLayout()
        );

        m_descriptorSets =
            vk::raii::DescriptorSets(m_device->GetDevice(), {m_device->GetDescriptorPool(), descriptorSetLayouts});

        for (uint32_t i = 0; i < viewer->GetMaximumOfFrames(); ++i)
        {
            vk::DescriptorBufferInfo dbInfo(m_uniformBufferObjects[i].GetBuffer(), 0, sizeof(m_color));
            std::array<vk::WriteDescriptorSet, 1> writeDescriptorSets {
                vk::WriteDescriptorSet {m_descriptorSets[i], 0, 0, vk::DescriptorType::eUniformBuffer, nullptr, dbInfo}
            };
            m_device->GetDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
        }
    }
}

void Mapper::SetColorMode(ColorMode colorMode) noexcept
{
    m_colorMode = colorMode;
}

ColorMode Mapper::GetColorMode() const noexcept
{
    return m_colorMode;
}

void Mapper::SetColor(const std::array<float, 3>& color)
{
    m_color = color;
}

std::array<float, 3> Mapper::GetColor() const noexcept
{
    return m_color;
}
