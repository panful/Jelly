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
#include "Object.h"
#include "Pipeline.h"
#include <array>
#include <atomic>
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;

enum class ColorMode : uint8_t
{
    VertexColoring, // 顶点着色
    UniformColor,   // 单色填充
    ColorMap,       // 颜色映射表
};

class JELLY_EXPORT Mapper : public Object
{
public:
    virtual void
    Render(const vk::raii::CommandBuffer& commandBuffer, const vk::raii::RenderPass& renderPass) noexcept = 0;

    void SetDevice(std::shared_ptr<Device> device) noexcept;

    void SetColorMode(ColorMode colorMode) noexcept;
    ColorMode GetColorMode() const noexcept;

    void SetColor(const std::array<double, 3>& color);
    std::array<double, 3> GetColor() const noexcept;

protected:
    void BuildPipeline(const vk::raii::RenderPass& renderPass, const PipelineInfo& pipelineInfo) noexcept;

protected:
    std::shared_ptr<Device> m_device {};
    std::unique_ptr<Drawable> m_drawable {};

    size_t m_pipelineKey {};

    std::atomic_bool m_needUpdate {true};

    ColorMode m_colorMode {ColorMode::UniformColor};
    std::array<double, 3> m_color {1., 1., 1.};
};
} // namespace Jelly
