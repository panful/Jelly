#include "ImageData.h"
#include "Device.h"
#include "MemoryHelper.h"

using namespace Jelly;

ImageData::ImageData(
    std::shared_ptr<Device> device,
    vk::Format format,
    const vk::Extent2D& extent,
    vk::ImageTiling tiling,
    vk::ImageUsageFlags usage,
    vk::ImageLayout initialLayout,
    vk::MemoryPropertyFlags memoryProperties,
    vk::ImageAspectFlags aspectMask,
    bool createImageView
)
    : m_image(
          device->GetDevice(),
          {vk::ImageCreateFlags(),
           vk::ImageType::e2D,
           format,
           vk::Extent3D(extent, 1),
           1,
           1,
           vk::SampleCountFlagBits::e1,
           tiling,
           usage,
           vk::SharingMode::eExclusive,
           {},
           initialLayout}
      )
{
    m_deviceMemory = MemoryHelper::AllocateDeviceMemory(device, m_image.getMemoryRequirements(), memoryProperties);
    m_image.bindMemory(m_deviceMemory, 0);
    m_imageView = createImageView
        ? vk::raii::ImageView(
              device->GetDevice(),
              vk::ImageViewCreateInfo({}, m_image, vk::ImageViewType::e2D, format, {}, {aspectMask, 0, 1, 0, 1})
          )
        : nullptr;
}

ImageData::ImageData(std::nullptr_t)
{
}

const vk::raii::Image& ImageData::GetImage() const noexcept
{
    return m_image;
}

const vk::raii::ImageView& ImageData::GetImageView() const noexcept
{
    return m_imageView;
}

const vk::raii::DeviceMemory& ImageData::GetDeviceMemory() const noexcept
{
    return m_deviceMemory;
}

void ImageData::SetImageLayout(
    const vk::raii::CommandBuffer& commandBuffer,
    vk::Image image,
    vk::Format format,
    vk::ImageLayout oldImageLayout,
    vk::ImageLayout newImageLayout
)
{
    vk::AccessFlags sourceAccessMask;
    switch (oldImageLayout)
    {
        case vk::ImageLayout::eTransferDstOptimal:
            sourceAccessMask = vk::AccessFlagBits::eTransferWrite;
            break;
        case vk::ImageLayout::ePreinitialized:
            sourceAccessMask = vk::AccessFlagBits::eHostWrite;
            break;
        case vk::ImageLayout::ePresentSrcKHR:
            sourceAccessMask = vk::AccessFlagBits::eMemoryRead;
            break;
        case vk::ImageLayout::eTransferSrcOptimal:
            sourceAccessMask = vk::AccessFlagBits::eTransferRead;
            break;
        case vk::ImageLayout::eGeneral:
        case vk::ImageLayout::eUndefined:
            sourceAccessMask = vk::AccessFlagBits::eNone;
            break;
        default:
            assert(false);
            break;
    }

    vk::PipelineStageFlags sourceStage;
    switch (oldImageLayout)
    {
        case vk::ImageLayout::eGeneral:
        case vk::ImageLayout::ePreinitialized:
            sourceStage = vk::PipelineStageFlagBits::eHost;
            break;
        case vk::ImageLayout::eTransferDstOptimal:
            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            break;
        case vk::ImageLayout::eUndefined:
            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            // sourceStage = vk::PipelineStageFlagBits::eTransfer;
            break;
        case vk::ImageLayout::ePresentSrcKHR:
            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            break;
        case vk::ImageLayout::eTransferSrcOptimal:
            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            break;
        default:
            assert(false);
            break;
    }

    vk::AccessFlags destinationAccessMask;
    switch (newImageLayout)
    {
        case vk::ImageLayout::eColorAttachmentOptimal:
            destinationAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
            break;
        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
            destinationAccessMask =
                vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
            break;
        case vk::ImageLayout::eGeneral:
        case vk::ImageLayout::ePresentSrcKHR:
            destinationAccessMask = vk::AccessFlagBits::eMemoryRead;
            break;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
            destinationAccessMask = vk::AccessFlagBits::eShaderRead;
            break;
        case vk::ImageLayout::eTransferSrcOptimal:
            destinationAccessMask = vk::AccessFlagBits::eTransferRead;
            break;
        case vk::ImageLayout::eTransferDstOptimal:
            destinationAccessMask = vk::AccessFlagBits::eTransferWrite;
            break;
        default:
            assert(false);
            break;
    }

    vk::PipelineStageFlags destinationStage;
    switch (newImageLayout)
    {
        case vk::ImageLayout::eColorAttachmentOptimal:
            destinationStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            break;
        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
            destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
            break;
        case vk::ImageLayout::eGeneral:
            destinationStage = vk::PipelineStageFlagBits::eHost;
            // destinationStage = vk::PipelineStageFlagBits::eTransfer;
            break;
        case vk::ImageLayout::ePresentSrcKHR:
            destinationStage = vk::PipelineStageFlagBits::eBottomOfPipe;
            // destinationStage = vk::PipelineStageFlagBits::eTransfer;
            break;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
            destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
            break;
        case vk::ImageLayout::eTransferDstOptimal:
        case vk::ImageLayout::eTransferSrcOptimal:
            destinationStage = vk::PipelineStageFlagBits::eTransfer;
            break;
        default:
            assert(false);
            break;
    }

    vk::ImageAspectFlags aspectMask;
    if (newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
    {
        aspectMask = vk::ImageAspectFlagBits::eDepth;
        if (format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint)
        {
            aspectMask |= vk::ImageAspectFlagBits::eStencil;
        }
    }
    else
    {
        aspectMask = vk::ImageAspectFlagBits::eColor;
    }

    vk::ImageSubresourceRange imageSubresourceRange(aspectMask, 0, 1, 0, 1);
    vk::ImageMemoryBarrier imageMemoryBarrier(
        sourceAccessMask,
        destinationAccessMask,
        oldImageLayout,
        newImageLayout,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        image,
        imageSubresourceRange
    );
    return commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, nullptr, nullptr, imageMemoryBarrier);
}
