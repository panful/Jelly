#include "Mapper.h"
#include "Device.h"
#include "Drawable.h"
#include "Renderer.h"
#include "Texture.h"
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
    if (ColorMode::Texture == m_colorMode || ColorMode::Uniform == m_colorMode)
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

    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(
        viewer->GetMaximumOfFrames(), m_device->GetPipelineCache()->GetPipeline(m_pipelineKey)->GetDescriptorSetLayout()
    );

    m_descriptorSets =
        vk::raii::DescriptorSets(m_device->GetDevice(), {m_device->GetDescriptorPool(), descriptorSetLayouts});

    if (ColorMode::Texture == m_colorMode)
    {
        vk::DescriptorImageInfo descriptorImageInfo(
            m_texture->GetSampler(), m_texture->GetImageData().GetImageView(), vk::ImageLayout::eShaderReadOnlyOptimal
        );

        for (uint32_t i = 0; i < viewer->GetMaximumOfFrames(); ++i)
        {
            std::array<vk::WriteDescriptorSet, 1> writeDescriptorSets {
                vk::WriteDescriptorSet {
                                        m_descriptorSets[i],
                                        pipelineInfo.descriptorSetLayoutBindings[0].binding,
                                        0, pipelineInfo.descriptorSetLayoutBindings[0].descriptorType,
                                        descriptorImageInfo, nullptr
                }
            };
            m_device->GetDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
        }
    }

    if (ColorMode::Uniform == m_colorMode)
    {
        m_uniformBufferObjects.reserve(viewer->GetMaximumOfFrames());
        for (uint32_t i = 0; i < viewer->GetMaximumOfFrames(); ++i)
        {
            m_uniformBufferObjects.emplace_back(
                m_device,
                sizeof(m_color),
                vk::BufferUsageFlagBits::eUniformBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                true
            );

            std::memcpy(m_uniformBufferObjects[i].GetMemoryPointer(), m_color.data(), sizeof(m_color));
        }

        for (uint32_t i = 0; i < viewer->GetMaximumOfFrames(); ++i)
        {
            vk::DescriptorBufferInfo dbInfo(m_uniformBufferObjects[i].GetBuffer(), 0, sizeof(m_color));

            // XXX descriptorSetLayoutBindings的索引后面需要更改，暂时只有一个(Uniform Texture)
            std::array<vk::WriteDescriptorSet, 1> writeDescriptorSets {
                vk::WriteDescriptorSet {
                                        m_descriptorSets[i],
                                        pipelineInfo.descriptorSetLayoutBindings[0].binding,
                                        0, pipelineInfo.descriptorSetLayoutBindings[0].descriptorType,
                                        nullptr, dbInfo
                }
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

void Mapper::SetTexture(std::shared_ptr<Texture> texture)
{
    m_texture = std::move(texture);
}

void Mapper::SetEnableLighting(bool enableLighting) noexcept
{
    m_enableLighting = enableLighting;
}

bool Mapper::GetEnableLighting() const noexcept
{
    return m_enableLighting;
}
