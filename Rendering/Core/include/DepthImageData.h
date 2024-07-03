/**
 * @file DepthImageData.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "ImageData.h"

namespace Jelly {
class JELLY_EXPORT DepthImageData : public ImageData
{
public:
    DepthImageData(std::shared_ptr<Device> device, vk::Format format, const vk::Extent2D& extent);

    DepthImageData(std::nullptr_t nullptrValue);
};
} // namespace Jelly
