/**
 * @file MemoryHelper.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Device.h"
#include "Logger.h"
#include <format>
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class MemoryHelper
{
public:
    template <typename T>
    static void CopyToDevice(
        const vk::raii::DeviceMemory& deviceMemory, const T* pData, size_t count, vk::DeviceSize stride = sizeof(T)
    )
    {
        if (sizeof(T) > stride)
        {
            Logger::GetInstance()->Error(std::format("data size is {}, but stride is {}", sizeof(T), stride));
        }

        uint8_t* deviceData = static_cast<uint8_t*>(deviceMemory.mapMemory(0, count * stride));
        if (stride == sizeof(T))
        {
            std::memcpy(deviceData, pData, count * sizeof(T));
        }
        else
        {
            for (size_t i = 0; i < count; i++)
            {
                std::memcpy(deviceData, &pData[i], sizeof(T));
                deviceData += stride;
            }
        }
        deviceMemory.unmapMemory();
    }

    template <typename T>
    static void CopyToDevice(const vk::raii::DeviceMemory& deviceMemory, const T& data)
    {
        CopyToDevice<T>(deviceMemory, &data, 1);
    }

    template <typename Func>
    static void OneTimeSubmit(const std::shared_ptr<Device>& device, const Func& func)
    {
        vk::raii::CommandPool commandPool(
            device->GetDevice(), {{vk::CommandPoolCreateFlagBits::eTransient}, device->GetGraphicsQueueIndex()}
        );

        vk::raii::CommandBuffer commandBuffer = std::move(
            vk::raii::CommandBuffers(device->GetDevice(), {commandPool, vk::CommandBufferLevel::ePrimary, 1}).front()
        );

        commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
        func(commandBuffer);
        commandBuffer.end();

        vk::SubmitInfo submitInfo(nullptr, nullptr, *commandBuffer);
        device->GetGraphicsQueue().submit(submitInfo, nullptr);
        device->GetGraphicsQueue().waitIdle();
    }

    static uint32_t FindMemoryType(
        const vk::PhysicalDeviceMemoryProperties& memoryProperties,
        vk::MemoryPropertyFlags requirementsMask,
        uint32_t typeBits
    ) noexcept;

    static vk::raii::DeviceMemory AllocateDeviceMemory(
        const std::shared_ptr<Device> device,
        const vk::MemoryRequirements& memoryRequirements,
        vk::MemoryPropertyFlags memoryPropertyFlags
    ) noexcept;
};
} // namespace Jelly
