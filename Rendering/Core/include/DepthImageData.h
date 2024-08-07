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
    DepthImageData(vk::Format format, vk::SampleCountFlagBits sampleCount, const vk::Extent2D& extent);
};
} // namespace Jelly
