/**
 * @file ImageData.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;

class JELLY_EXPORT ImageData : public Object
{
public:
    ImageData(
        std::shared_ptr<Device> device,
        vk::Format format,
        const vk::Extent2D& extent,
        vk::ImageTiling tiling,
        vk::ImageUsageFlags usage,
        vk::ImageLayout initialLayout,
        vk::MemoryPropertyFlags memoryProperties,
        vk::ImageAspectFlags aspectMask,
        bool createImageView = true
    );

    const vk::raii::Image& GetImage() const noexcept;
    const vk::raii::ImageView& GetImageView() const noexcept;
    const vk::raii::DeviceMemory& GetDeviceMemory() const noexcept;

    static void SetImageLayout(
        const vk::raii::CommandBuffer& commandBuffer,
        vk::Image image,
        vk::Format format,
        vk::ImageLayout oldImageLayout,
        vk::ImageLayout newImageLayout
    );

private:
    vk::raii::Image m_image {nullptr};
    vk::raii::ImageView m_imageView {nullptr};
    vk::raii::DeviceMemory m_deviceMemory {nullptr};
};
} // namespace Jelly
