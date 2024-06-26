/**
 * @file BufferData.h
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
#include "MemoryHelper.h"
#include <format>
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class BufferData
{
public:
    BufferData(std::nullptr_t) noexcept;

    BufferData(
        std::shared_ptr<Device> device,
        vk::DeviceSize size,
        vk::BufferUsageFlags usage,
        vk::MemoryPropertyFlags propertyFlags
    );

    vk::Buffer GetBuffer() const noexcept;

    /// @brief 将CPU端的数据拷贝到GPU缓冲
    /// @details 保证数据被立即复制到缓冲关联的内存
    /// @tparam DataType 拷贝的数据结构类型
    /// @param data
    template <typename DataType>
    void Upload(const DataType& data) const;

    /// @brief 将CPU端数据拷贝到GPU缓冲
    /// @details 不保证数据被立即复制到缓冲关联的内存，当stride不等于0时，stride应该不小于DataType的大小
    /// @tparam DataType 拷贝的数据结构类型
    /// @param data
    /// @param stride 数据的步长，即拷贝一次的数据大小，默认是 sizeof(DataType)
    template <typename DataType>
    void Upload(const std::vector<DataType>& data, size_t stride = 0) const;

    /// @brief 使用暂存缓冲将CPU端的数据拷贝到GPU
    /// @tparam DataType
    /// @param device
    /// @param data
    /// @param stride 数据的步长
    template <typename DataType>
    void Upload(const std::shared_ptr<Device> device, const std::vector<DataType>& data, size_t stride = 0) const;

private:
    vk::raii::DeviceMemory m_deviceMemory {nullptr};
    vk::raii::Buffer m_buffer {nullptr};

    vk::DeviceSize m_size {};
    vk::BufferUsageFlags m_usage {};
    vk::MemoryPropertyFlags m_propertyFlags {};
};

template <typename DataType>
void BufferData::Upload(const DataType& data) const
{
    if (!((m_propertyFlags & vk::MemoryPropertyFlagBits::eHostCoherent)
          && (m_propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible)))
    {
        Logger::GetInstance()->Error("Memory property error");
    }

    if (sizeof(DataType) > m_size)
    {
        Logger::GetInstance()->Error(
            std::format("Data size error, need size is {}, but current size is {}", m_size, sizeof(DataType))
        );
    }

    void* dataPtr = m_deviceMemory.mapMemory(0, sizeof(DataType));
    std::memcpy(dataPtr, &data, sizeof(DataType));
    m_deviceMemory.unmapMemory();
}

template <typename DataType>
void BufferData::Upload(const std::vector<DataType>& data, size_t stride) const
{
    if (!(m_propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible))
    {
        Logger::GetInstance()->Error("Memory property error");
    }

    size_t elementSize = stride ? stride : sizeof(DataType);

    if (sizeof(DataType) > elementSize)
    {
        Logger::GetInstance()->Error(
            std::format("Data size error, need size is {}, but current size is {}", elementSize, sizeof(DataType))
        );
    }

    MemoryHelper::CopyToDevice(m_deviceMemory, data.data(), data.size(), elementSize);
}

template <typename DataType>
void BufferData::Upload(const std::shared_ptr<Device> device, const std::vector<DataType>& data, size_t stride) const
{
    if (!(m_usage & vk::BufferUsageFlagBits::eTransferDst))
    {
        Logger::GetInstance()->Error(
            "Buffer usage error, if use stagingbuffer, the m_usage should be vk::BufferUsageFlagBits::eTransferDst"
        );
    }

    if (!(m_propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal))
    {
        Logger::GetInstance()->Error("Memory property error, if use stagingbuffer, the m_propertyFlags should be "
                                     "vk::MemoryPropertyFlagBits::eDeviceLocal");
    }

    size_t elementSize = stride ? stride : sizeof(DataType);

    if (sizeof(DataType) > elementSize)
    {
        Logger::GetInstance()->Error(
            std::format("Data size error, need size is {}, but current size is {}", elementSize, sizeof(DataType))
        );
    }

    size_t dataSize = data.size() * elementSize;

    if (dataSize > m_size)
    {
        Logger::GetInstance()->Error(
            std::format("Data size error, need size is {}, but current size is {}", m_size, dataSize)
        );
    }

    BufferData stagingBuffer(
        device,
        dataSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    MemoryHelper::CopyToDevice(stagingBuffer.m_deviceMemory, data.data(), data.size(), elementSize);

    MemoryHelper::OneTimeSubmit(device, [&](const vk::raii::CommandBuffer& commandBuffer) {
        commandBuffer.copyBuffer(*stagingBuffer.m_buffer, *this->m_buffer, vk::BufferCopy(0, 0, dataSize));
    });
}
} // namespace Jelly
