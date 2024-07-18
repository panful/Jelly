/**
 * @file Texture.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "ImageData.h"
#include "Object.h"
#include <memory>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;

class JELLY_EXPORT Texture : public Object
{
public:
    /// @brief
    /// @param extent
    /// @param pixelData 像素数据，格式为：RGBA
    void SetImage(const vk::Extent2D& extent, std::vector<uint8_t>&& pixelData);

    void SetDevice(std::shared_ptr<Device> device);

    void Update();

    const vk::raii::Sampler& GetSampler() const noexcept;

    const std::unique_ptr<ImageData>& GetImageData() const noexcept;

private:
    vk::Format m_format {vk::Format::eR8G8B8A8Unorm};
    vk::Extent2D m_extent {256, 256};
    std::shared_ptr<Device> m_device {};
    std::unique_ptr<ImageData> m_imageData {};
    vk::raii::Sampler m_sampler {nullptr};
    std::vector<uint8_t> m_pixelData {};
};
} // namespace Jelly
