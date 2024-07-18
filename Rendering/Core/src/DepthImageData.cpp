#include "DepthImageData.h"

using namespace Jelly;

DepthImageData::DepthImageData(std::shared_ptr<Device> device, vk::Format format, const vk::Extent2D& extent)
    : ImageData(
          std::move(device),
          format,
          extent,
          vk::ImageTiling::eOptimal,
          vk::ImageUsageFlagBits::eDepthStencilAttachment,
          vk::ImageLayout::eUndefined,
          vk::MemoryPropertyFlagBits::eDeviceLocal,
          vk::ImageAspectFlagBits::eDepth
      )
{
}
