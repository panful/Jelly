#include "BufferData.h"

using namespace Jelly;

BufferData::BufferData(std::nullptr_t) noexcept
{
}

BufferData::BufferData(
    std::shared_ptr<Device> device,
    vk::DeviceSize size,
    vk::BufferUsageFlags usage,
    vk::MemoryPropertyFlags propertyFlags,
    bool mapMemory
)
    : m_buffer(device->GetDevice(), vk::BufferCreateInfo({}, size, usage))
    , m_size(size)
    , m_usage(usage)
    , m_propertyFlags(propertyFlags)
{
    m_deviceMemory = MemoryHelper::AllocateDeviceMemory(device, m_buffer.getMemoryRequirements(), propertyFlags);
    m_buffer.bindMemory(m_deviceMemory, 0);

    if (mapMemory)
    {
        m_memoryPointer = m_deviceMemory.mapMemory(0, m_size);
    }
}

BufferData::~BufferData() noexcept
{
    if (m_memoryPointer)
    {
        m_deviceMemory.unmapMemory();
        m_memoryPointer = nullptr;
    }
}

const vk::raii::Buffer& BufferData::GetBuffer() const noexcept
{
    return m_buffer;
}

const vk::raii::DeviceMemory& BufferData::GetDeviceMemory() const noexcept
{
    return m_deviceMemory;
}

void* BufferData::GetMemoryPointer() const noexcept
{
    return m_memoryPointer;
}
