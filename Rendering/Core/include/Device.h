/**
 * @file Device.h
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
#include <tuple>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {

class JELLY_EXPORT Device : public Object
{
public:
    void SetEnableValidationLayers(bool enable);

    bool CheckSurfaceSupport(const vk::raii::SurfaceKHR& surface);

    const vk::raii::Instance& GetInstance() const noexcept;

public:
    vk::Instance InitInstance() noexcept;

    vk::PhysicalDevice PickPhysicalDevice(const vk::raii::SurfaceKHR& surface) noexcept;

    vk::Device InitDevice() noexcept;

    std::pair<vk::Queue, vk::Queue> InitQueues() noexcept;

private:
    void CreateDebugUtilsMessengerEXT() noexcept;

    /// @brief
    /// @param physicalDevice
    /// @param surface
    /// @return 0 是否符合要求 1 图形队列簇序号 2 展示队列簇序号
    std::tuple<bool, uint32_t, uint32_t> IsDeviceSuitable(
        const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::SurfaceKHR& surface
    ) const noexcept;

private:
    vk::raii::Context m_context {};
    vk::raii::Instance m_instance {nullptr};
    vk::raii::DebugUtilsMessengerEXT m_debugUtilsMessengerEXT {nullptr};

    vk::raii::PhysicalDevice m_physicalDevice {nullptr};
    vk::raii::Device m_device {nullptr};

    vk::raii::Queue m_graphicsQueue {nullptr};
    vk::raii::Queue m_presentQueue {nullptr};

    uint32_t m_graphicsQueueIndex {};
    uint32_t m_presentQueueIndex {};

private:
    inline static const std::string m_appName {"Jelly"};
    inline static const std::string m_engineName {"Jelly"};

    bool m_enableValidationLayers {true};

    std::vector<const char*> m_enableLayerNames {"VK_LAYER_KHRONOS_validation"};
    std::vector<const char*> m_enableInstanceExtensionNames {};
    std::vector<const char*> m_enableDeviceExtensionNames {};
};
} // namespace Jelly
