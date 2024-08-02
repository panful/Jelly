#include "MemoryHelper.h"

using namespace Jelly;

uint32_t MemoryHelper::FindMemoryType(
    const vk::PhysicalDeviceMemoryProperties& memoryProperties,
    vk::MemoryPropertyFlags requirementsMask,
    uint32_t typeBits
) noexcept
{
    uint32_t typeIndex = uint32_t(~0);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if ((typeBits & 1) && ((memoryProperties.memoryTypes[i].propertyFlags & requirementsMask) == requirementsMask))
        {
            typeIndex = i;
            break;
        }
        typeBits >>= 1;
    }

    if (typeIndex == uint32_t(~0))
    {
        Logger::GetInstance()->Error("typeIndex == uint32_t(~0)");
    }

    return typeIndex;
}

vk::raii::DeviceMemory MemoryHelper::AllocateDeviceMemory(
    const vk::MemoryRequirements& memoryRequirements,
    vk::MemoryPropertyFlags memoryPropertyFlags
) noexcept
{
    uint32_t memoryTypeIndex = FindMemoryType(
        Device::Get()->GetPhysicalDevice().getMemoryProperties(), memoryPropertyFlags, memoryRequirements.memoryTypeBits
    );
    vk::MemoryAllocateInfo memoryAllocateInfo(memoryRequirements.size, memoryTypeIndex);

    return vk::raii::DeviceMemory(Device::Get()->GetDevice(), memoryAllocateInfo);
}
