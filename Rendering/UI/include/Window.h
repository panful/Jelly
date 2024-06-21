/**
 * @file Window.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#pragma warning(disable : 4251)

#include "Object.h"
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Renderer;

class JELLY_EXPORT Window : public Object
{
public:
    Window() noexcept;

    void AddRenderer(std::shared_ptr<Renderer> renderer);

    virtual void Render() noexcept = 0;

    virtual void* GetNativeWindow() const noexcept = 0;

    void SetSize(const uint32_t width, const uint32_t height) noexcept;
    void SetTitle(const std::string_view title) noexcept;

protected:
    uint32_t m_width {800};
    uint32_t m_height {600};
    std::string m_title {"Jelly"};
    std::shared_ptr<Device> m_device {};
    vk::raii::SurfaceKHR m_surface {nullptr};
    std::vector<std::shared_ptr<Renderer>> m_renderers {};
};
} // namespace Jelly
