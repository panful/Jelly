/**
 * @file Renderer.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#pragma warning(disable : 4251)

#include "Object.h"
#include <array>
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Window;

class JELLY_EXPORT Renderer : public Object
{
public:
    void Render(const vk::raii::CommandBuffer& commandBuffer) noexcept;

    void SetWindow(std::shared_ptr<Window> window);
    void SetViewport(const std::array<double, 4>& viewport);
    void SetBackground(const std::array<float, 4>& background);

private:
    std::shared_ptr<Device> m_device {};
    std::weak_ptr<Window> m_window {};

    std::array<double, 4> m_viewport {0., 0., 1., 1.}; // 起始位置以及宽高
    std::array<float, 4> m_background {.3f, .2f, .1f, 1.f};
};
} // namespace Jelly
