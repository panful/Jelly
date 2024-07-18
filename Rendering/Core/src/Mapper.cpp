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
    if (IsChanged())
    {
        Configure(viewer);
        ResetChanged();
    }

    DeviceRender(commandBuffer, viewer, renderer);
}

void Mapper::DeviceRender(
    const vk::raii::CommandBuffer& commandBuffer, const std::shared_ptr<Viewer>& viewer, Renderer* renderer
)
{
    auto&& pipeline = m_device->GetPipelineCache()->GetPipeline(m_pipelineKey);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());

    switch (m_colorMode)
    {
        case ColorMode::Texture:
            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipeline->GetPipelineLayout(),
                0,
                {m_textureColorDescriptorSets->descriptorSets[viewer->GetCurrentFrameIndex()]},
                nullptr
            );
            break;
        case ColorMode::Uniform:
            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipeline->GetPipelineLayout(),
                0,
                {m_uniformColorDescriptorSets->descriptorSets[viewer->GetCurrentFrameIndex()]},
                nullptr
            );
            break;
        default:
            break;
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

    switch (m_colorMode)
    {
        case ColorMode::Texture:
            if (!m_textureColorDescriptorSets)
            {
                m_textureColorDescriptorSets = std::make_unique<PrivateDescriptorSets>();
            }
            if (!m_textureColorDescriptorSets->initialized)
            {
                std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(
                    viewer->GetMaximumOfFrames(),
                    m_device->GetPipelineCache()->GetPipeline(m_pipelineKey)->GetDescriptorSetLayout()
                );

                m_textureColorDescriptorSets->descriptorSets = vk::raii::DescriptorSets(
                    m_device->GetDevice(), {m_device->GetDescriptorPool(), descriptorSetLayouts}
                );

                vk::DescriptorImageInfo descriptorImageInfo(
                    m_texture->GetSampler(),
                    m_texture->GetImageData()->GetImageView(),
                    vk::ImageLayout::eShaderReadOnlyOptimal
                );

                for (uint32_t i = 0; i < viewer->GetMaximumOfFrames(); ++i)
                {
                    // XXX descriptorSetLayoutBindings的索引后面需要更改，暂时只有一个(Uniform Texture)
                    std::array<vk::WriteDescriptorSet, 1> writeDescriptorSets {
                        vk::WriteDescriptorSet {
                                                m_textureColorDescriptorSets->descriptorSets[i],
                                                pipelineInfo.descriptorSetLayoutBindings[0].binding,
                                                0, pipelineInfo.descriptorSetLayoutBindings[0].descriptorType,
                                                descriptorImageInfo, nullptr
                        }
                    };
                    m_device->GetDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
                }

                m_textureColorDescriptorSets->initialized = true;
            }
            break;
        case ColorMode::Uniform:
            if (!m_uniformColorDescriptorSets)
            {
                m_uniformColorDescriptorSets = std::make_unique<PrivateDescriptorSets>();
            }
            if (!m_uniformColorDescriptorSets->initialized)
            {
                std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(
                    viewer->GetMaximumOfFrames(),
                    m_device->GetPipelineCache()->GetPipeline(m_pipelineKey)->GetDescriptorSetLayout()
                );

                m_uniformColorDescriptorSets->descriptorSets = vk::raii::DescriptorSets(
                    m_device->GetDevice(), {m_device->GetDescriptorPool(), descriptorSetLayouts}
                );

                m_uniformBufferObjects.resize(viewer->GetMaximumOfFrames());
                for (uint32_t i = 0; i < viewer->GetMaximumOfFrames(); ++i)
                {
                    m_uniformBufferObjects[i] = std::make_unique<BufferData>(
                        m_device,
                        sizeof(m_color),
                        vk::BufferUsageFlagBits::eUniformBuffer,
                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                        true
                    );

                    std::memcpy(m_uniformBufferObjects[i]->GetMemoryPointer(), m_color.data(), sizeof(m_color));
                }

                for (uint32_t i = 0; i < viewer->GetMaximumOfFrames(); ++i)
                {
                    vk::DescriptorBufferInfo dbInfo(m_uniformBufferObjects[i]->GetBuffer(), 0, sizeof(m_color));

                    // XXX descriptorSetLayoutBindings的索引后面需要更改，暂时只有一个(Uniform Texture)
                    std::array<vk::WriteDescriptorSet, 1> writeDescriptorSets {
                        vk::WriteDescriptorSet {
                                                m_uniformColorDescriptorSets->descriptorSets[i],
                                                pipelineInfo.descriptorSetLayoutBindings[0].binding,
                                                0, pipelineInfo.descriptorSetLayoutBindings[0].descriptorType,
                                                nullptr, dbInfo
                        }
                    };
                    m_device->GetDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
                }

                m_uniformColorDescriptorSets->initialized = true;
            }
            break;
        default:
            break;
    }
}

void Mapper::SetColorMode(ColorMode colorMode) noexcept
{
    if (m_colorMode != colorMode)
    {
        m_colorMode = colorMode;
        Changed();
    }
}

ColorMode Mapper::GetColorMode() const noexcept
{
    return m_colorMode;
}

void Mapper::SetColor(const std::array<float, 3>& color)
{
    if (m_color != color)
    {
        m_color = color;
        Changed();
    }
}

const std::array<float, 3>& Mapper::GetColor() const noexcept
{
    return m_color;
}

void Mapper::SetTexture(std::shared_ptr<Texture> texture)
{
    if (m_texture != texture)
    {
        m_texture = std::move(texture);
        Changed();
    }
}

void Mapper::SetEnableLighting(bool enableLighting) noexcept
{
    if (m_enableLighting != enableLighting)
    {
        m_enableLighting = enableLighting;
        Changed();
    }
}

bool Mapper::GetEnableLighting() const noexcept
{
    return m_enableLighting;
}
