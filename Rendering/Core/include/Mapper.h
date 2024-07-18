/**
 * @file Mapper.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "BufferData.h"
#include "Drawable.h"
#include "Object.h"
#include "Pipeline.h"
#include <array>
#include <memory>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Actor;
class Renderer;
class Texture;

enum class ColorMode : uint8_t
{
    Vertex,  // 顶点着色
    Uniform, // 单色填充
    Texture, // 2D纹理映射
};

class JELLY_EXPORT Mapper : public Object
{
    struct PrivateDescriptorSets
    {
        bool initialized {false};
        vk::raii::DescriptorSets descriptorSets {nullptr};
    };

public:
    void Render(const vk::raii::CommandBuffer& commandBuffer, Renderer* renderer, Actor* actor) noexcept;

    virtual void Update(uint32_t maximumOfFrames, const vk::raii::RenderPass& renderPass) noexcept = 0;

    virtual std::array<double, 6> GetBounds() const noexcept = 0;

    void SetDevice(std::shared_ptr<Device> device) noexcept;

    void SetColorMode(ColorMode colorMode) noexcept;
    ColorMode GetColorMode() const noexcept;

    void SetColor(const std::array<float, 3>& color);
    const std::array<float, 3>& GetColor() const noexcept;

    void SetTexture(std::shared_ptr<Texture> texture);

    void SetEnableLighting(bool enableLighting) noexcept;
    bool GetEnableLighting() const noexcept;

protected:
    void BuildPipeline(uint32_t maximumOfFrames, const PipelineInfo& pipelineInfo) noexcept;

private:
    void DeviceRender(
        const vk::raii::CommandBuffer& commandBuffer, Renderer* renderer, Actor* actor, uint32_t currentFrameIndex
    );

protected:
    std::shared_ptr<Device> m_device {};
    std::unique_ptr<Drawable> m_drawable {};

    size_t m_pipelineKey {};
    bool m_enableLighting {true};

    std::array<float, 3> m_color {1., 1., 1.};
    ColorMode m_colorMode {ColorMode::Uniform};
    std::unique_ptr<PrivateDescriptorSets> m_uniformColorDescriptorSets {};
    std::unique_ptr<PrivateDescriptorSets> m_textureColorDescriptorSets {};
    std::vector<std::unique_ptr<BufferData>> m_uniformBufferObjects {};
    std::shared_ptr<Texture> m_texture {};
};
} // namespace Jelly
