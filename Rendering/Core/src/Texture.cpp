#include "Texture.h"
#include "BufferData.h"
#include "Device.h"
#include "MemoryHelper.h"

using namespace Jelly;

void Texture::SetImage(const vk::Extent2D& extent, std::vector<uint8_t>&& pixelData)
{
    m_extent    = extent;
    m_pixelData = std::move(pixelData);
}

void Texture::SetDevice(std::shared_ptr<Device> device)
{
    m_device = std::move(device);
}

void Texture::Update()
{
    if (!IsChanged())
    {
        return;
    }

    auto imageSize = m_extent.width * m_extent.height * 4;

    BufferData stagingBufferData(
        m_device,
        imageSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    stagingBufferData.Upload(m_pixelData);

    m_imageData = std::make_unique<ImageData>(
        m_device,
        m_format,
        m_extent,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
        vk::ImageLayout::eUndefined,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        vk::ImageAspectFlagBits::eColor
    );

    MemoryHelper::OneTimeSubmit(m_device, [this, &stagingBufferData](const vk::raii::CommandBuffer& commandBuffer) {
        ImageData::SetImageLayout(
            commandBuffer,
            this->m_imageData->GetImage(),
            this->m_format,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferDstOptimal
        );

        vk::BufferImageCopy copyRegion(
            0,
            this->m_extent.width,
            this->m_extent.height,
            vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
            vk::Offset3D(0, 0, 0),
            vk::Extent3D(this->m_extent, 1)
        );
        commandBuffer.copyBufferToImage(
            stagingBufferData.GetBuffer(),
            this->m_imageData->GetImage(),
            vk::ImageLayout::eTransferDstOptimal,
            copyRegion
        );

        ImageData::SetImageLayout(
            commandBuffer,
            this->m_imageData->GetImage(),
            this->m_format,
            vk::ImageLayout::eTransferDstOptimal,
            vk::ImageLayout::eShaderReadOnlyOptimal
        );
    });

    m_sampler = vk::raii::Sampler(
        m_device->GetDevice(),
        {{},
         vk::Filter::eLinear,
         vk::Filter::eLinear,
         vk::SamplerMipmapMode::eLinear,
         vk::SamplerAddressMode::eMirroredRepeat,
         vk::SamplerAddressMode::eMirroredRepeat,
         vk::SamplerAddressMode::eMirroredRepeat,
         0.0f,
         false,
         16.0f,
         false,
         vk::CompareOp::eNever,
         0.0f,
         0.0f,
         vk::BorderColor::eFloatOpaqueBlack}
    );

    m_pixelData.clear();
    ResetChanged();
}

const vk::raii::Sampler& Texture::GetSampler() const noexcept
{
    return m_sampler;
}

const std::unique_ptr<ImageData>& Texture::GetImageData() const noexcept
{
    return m_imageData;
}
