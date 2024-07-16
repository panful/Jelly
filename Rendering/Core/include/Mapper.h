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

#include "Drawable.h"
#include "BufferData.h"
#include "Object.h"
#include "Pipeline.h"
#include <array>
#include <memory>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Viewer;
class Renderer;

enum class ColorMode : uint8_t
{
    VertexColoring, // 顶点着色
    UniformColor,   // 单色填充
    ColorMap,       // 颜色映射表
};

class JELLY_EXPORT Mapper : public Object
{
public:
    void Render(
        const vk::raii::CommandBuffer& commandBuffer, const std::shared_ptr<Viewer>& viewer, Renderer* renderer
    ) noexcept;

    virtual void Configure(const std::shared_ptr<Viewer>& viewer) noexcept = 0;

    virtual std::array<double, 6> GetBounds() const noexcept = 0;

    void SetDevice(std::shared_ptr<Device> device) noexcept;

    void SetColorMode(ColorMode colorMode) noexcept;
    ColorMode GetColorMode() const noexcept;

    void SetColor(const std::array<float, 3>& color);
    std::array<float, 3> GetColor() const noexcept;

    void SetEnableLighting(bool enableLighting) noexcept;
    bool GetEnableLighting() const noexcept;

protected:
    void BuildPipeline(const std::shared_ptr<Viewer>& viewer, const PipelineInfo& pipelineInfo) noexcept;

private:
    void DeviceRender(
        const vk::raii::CommandBuffer& commandBuffer, const std::shared_ptr<Viewer>& viewer, Renderer* renderer
    );

protected:
    std::shared_ptr<Device> m_device {};
    std::unique_ptr<Drawable> m_drawable {};

    size_t m_pipelineKey {};

    ColorMode m_colorMode {ColorMode::UniformColor};
    bool m_useUniformColor {false};
    std::array<float, 3> m_color {1., 1., 1.};
    vk::raii::DescriptorSets m_descriptorSets {nullptr};
    std::vector<BufferData> m_uniformBufferObjects {};
    bool m_enableLighting {true};
};
} // namespace Jelly
