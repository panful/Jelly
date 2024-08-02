#include "DepthImageData.h"

using namespace Jelly;

DepthImageData::DepthImageData(vk::Format format, vk::SampleCountFlagBits sampleCount, const vk::Extent2D& extent)
    : ImageData(
          format,
          extent,
          vk::ImageTiling::eOptimal,
          vk::ImageUsageFlagBits::eDepthStencilAttachment,
          vk::ImageLayout::eUndefined,
          vk::MemoryPropertyFlagBits::eDeviceLocal,
          vk::ImageAspectFlagBits::eDepth,
          sampleCount
      )
{
}
