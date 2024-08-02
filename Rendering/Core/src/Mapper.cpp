#include "Mapper.h"
#include "Actor.h"
#include "Device.h"
#include "Drawable.h"
#include "Renderer.h"
#include "Texture.h"
#include "Viewer.h"
#include <functional>

using namespace Jelly;

void Mapper::Render(const vk::raii::CommandBuffer& commandBuffer, Renderer* renderer, Actor* actor) noexcept
{
    if (auto viewer = renderer->GetViewer())
    {
        if (IsChanged())
        {
            Update(viewer->GetMaximumOfFrames(), viewer->GetRenderPass(), actor);
            ResetChanged();
        }

        DeviceRender(commandBuffer, renderer, actor, viewer->GetCurrentFrameIndex());
    }
}

void Mapper::DeviceRender(
    const vk::raii::CommandBuffer& commandBuffer, Renderer* renderer, Actor* actor, uint32_t currentFrameIndex
)
{
    auto&& pipeline = Device::Get()->GetPipelineCache()->GetPipeline(m_pipelineKey);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());

    switch (m_colorMode)
    {
        case ColorMode::Texture:
            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipeline->GetPipelineLayout(),
                0,
                {m_textureColorDescriptorSets->descriptorSets[currentFrameIndex]},
                nullptr
            );
            break;
        case ColorMode::Uniform:
            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipeline->GetPipelineLayout(),
                0,
                {m_uniformColorDescriptorSets->descriptorSets[currentFrameIndex]},
                nullptr
            );
            break;
        default:
            break;
    }

    auto camera = renderer->GetCamera();
    std::vector<std::array<float, 16>> transform {
        actor->GetModelMatrix(), camera->GetViewMatrix(), camera->GetProjectMatrix()
    };

    commandBuffer.pushConstants<std::array<float, 16>>(
        pipeline->GetPipelineLayout(), vk::ShaderStageFlagBits::eVertex, 0, std::move(transform)
    );
    commandBuffer.bindVertexBuffers(0, m_drawable->GetVertexBuffers(), m_drawable->GetVertexOffsets());
    commandBuffer.bindIndexBuffer(m_drawable->GetIndexBuffer(), 0, m_drawable->GetIndexType());
    commandBuffer.drawIndexed(m_drawable->GetIndexCount(), 1, 0, 0, 0);
}

void Mapper::BuildPipeline(uint32_t maximumOfFrames, const PipelineInfo& pipelineInfo, Actor* actor) noexcept
{
    m_pipelineKey = std::hash<PipelineInfo>()(pipelineInfo);

    if (!Device::Get()->GetPipelineCache()->HasPipeline(m_pipelineKey))
    {
        Device::Get()->GetPipelineCache()->AddPipeline(m_pipelineKey, std::make_unique<Pipeline>(pipelineInfo));
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
                    maximumOfFrames,
                    Device::Get()->GetPipelineCache()->GetPipeline(m_pipelineKey)->GetDescriptorSetLayout()
                );

                m_textureColorDescriptorSets->descriptorSets = vk::raii::DescriptorSets(
                    Device::Get()->GetDevice(), {Device::Get()->GetDescriptorPool(), descriptorSetLayouts}
                );

                vk::DescriptorImageInfo descriptorImageInfo(
                    actor->GetTexture()->GetSampler(),
                    actor->GetTexture()->GetImageData()->GetImageView(),
                    vk::ImageLayout::eShaderReadOnlyOptimal
                );

                for (uint32_t i = 0; i < maximumOfFrames; ++i)
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
                    Device::Get()->GetDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
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
                    maximumOfFrames,
                    Device::Get()->GetPipelineCache()->GetPipeline(m_pipelineKey)->GetDescriptorSetLayout()
                );

                m_uniformColorDescriptorSets->descriptorSets = vk::raii::DescriptorSets(
                    Device::Get()->GetDevice(), {Device::Get()->GetDescriptorPool(), descriptorSetLayouts}
                );

                auto colorSize = sizeof(actor->GetColor());
                m_uniformBufferObjects.resize(maximumOfFrames);
                for (uint32_t i = 0; i < maximumOfFrames; ++i)
                {
                    m_uniformBufferObjects[i] = std::make_unique<BufferData>(
                        colorSize,
                        vk::BufferUsageFlagBits::eUniformBuffer,
                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                        true
                    );

                    std::memcpy(m_uniformBufferObjects[i]->GetMemoryPointer(), actor->GetColor().data(), colorSize);
                }

                for (uint32_t i = 0; i < maximumOfFrames; ++i)
                {
                    vk::DescriptorBufferInfo dbInfo(m_uniformBufferObjects[i]->GetBuffer(), 0, colorSize);

                    // XXX descriptorSetLayoutBindings的索引后面需要更改，暂时只有一个(Uniform Texture)
                    std::array<vk::WriteDescriptorSet, 1> writeDescriptorSets {
                        vk::WriteDescriptorSet {
                                                m_uniformColorDescriptorSets->descriptorSets[i],
                                                pipelineInfo.descriptorSetLayoutBindings[0].binding,
                                                0, pipelineInfo.descriptorSetLayoutBindings[0].descriptorType,
                                                nullptr, dbInfo
                        }
                    };
                    Device::Get()->GetDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
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

const std::unique_ptr<Drawable>& Mapper::GetDrawable() const noexcept
{
    return m_drawable;
}
