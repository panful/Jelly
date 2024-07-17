#include "Device.h"
#include "Logger.h"
#include "PipelineCache.h"
#include <format>
#include <optional>
#include <set>

using namespace Jelly;

namespace {
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) noexcept
{
    Logger::GetInstance()->Debug(std::format("Vulkan: {}", pCallbackData->pMessage));
    return VK_FALSE;
}
} // namespace

vk::Instance Device::InitInstance() noexcept
{
    Logger::GetInstance()->Trace();
    if (*m_instance)
    {
        return m_instance;
    }

    m_enableInstanceExtensionNames.emplace_back("VK_KHR_surface");
// TODO
#if _WIN32
    m_enableInstanceExtensionNames.emplace_back("VK_KHR_win32_surface");
#endif // _WIN32

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags {
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
    };
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags {
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
        | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
    };

    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoExt {
        {}, severityFlags, messageTypeFlags, DebugCallback
    };

    vk::ApplicationInfo applicationInfo {m_appName.c_str(), 1, m_engineName.c_str(), 1, VK_API_VERSION_1_1};

    vk::InstanceCreateInfo instanceCreateInfo {};
    if (m_enableValidationLayers)
    {
        m_enableInstanceExtensionNames.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        instanceCreateInfo.setPEnabledLayerNames(m_enableLayerNames);
        instanceCreateInfo.setPNext(&debugUtilsMessengerCreateInfoExt);
    }
    instanceCreateInfo.setPApplicationInfo(&applicationInfo);
    instanceCreateInfo.setPEnabledExtensionNames(m_enableInstanceExtensionNames);

    m_instance = vk::raii::Instance(m_context, instanceCreateInfo);

    if (m_enableValidationLayers)
    {
        CreateDebugUtilsMessengerEXT();
    }

    return m_instance;
}

void Device::SetEnableValidationLayers(bool enable)
{
    m_enableValidationLayers = enable;
}

bool Device::CheckSurfaceSupport(const vk::raii::SurfaceKHR& surface)
{
    return false;
}

uint32_t Device::GetGraphicsQueueIndex() const noexcept
{
    return m_graphicsQueueIndex;
}

uint32_t Device::GetPresentQueueIndex() const noexcept
{
    return m_presentQueueIndex;
}

const vk::raii::Instance& Device::GetInstance() const noexcept
{
    return m_instance;
}

const vk::raii::PhysicalDevice& Device::GetPhysicalDevice() const noexcept
{
    return m_physicalDevice;
}

const vk::raii::Device& Device::GetDevice() const noexcept
{
    return m_device;
}

const vk::raii::Queue& Device::GetGraphicsQueue() const noexcept
{
    return m_graphicsQueue;
}

const vk::raii::Queue& Device::GetPresentQueue() const noexcept
{
    return m_presentQueue;
}

const vk::raii::CommandPool& Device::GetCommandPool() const noexcept
{
    return m_commandPool;
}

const vk::raii::DescriptorPool& Device::GetDescriptorPool() const noexcept
{
    return m_descriptorPool;
}

const std::unique_ptr<PipelineCache>& Device::GetPipelineCache() const noexcept
{
    return m_pipelineCache;
}

vk::PhysicalDevice Device::PickPhysicalDevice(const vk::raii::SurfaceKHR& surface) noexcept
{
    Logger::GetInstance()->Trace();
    if (*m_physicalDevice)
    {
        return m_physicalDevice;
    }

    vk::raii::PhysicalDevices physicalDevices(m_instance);
    for (const auto& physicalDevice : physicalDevices)
    {
        if (auto [suitable, gIndex, pIndex] = IsDeviceSuitable(physicalDevice, surface); suitable)
        {
            m_physicalDevice     = physicalDevice;
            m_graphicsQueueIndex = gIndex;
            m_presentQueueIndex  = pIndex;
            break;
        }
    }

    if (!(*m_physicalDevice))
    {
        Logger::GetInstance()->Error("Invalid physical device");
    }

    Logger::GetInstance()->Trace(
        std::format("Physical device name: {}", m_physicalDevice.getProperties().deviceName.data())
    );

    return m_physicalDevice;
}

vk::Device Device::InitDevice() noexcept
{
    Logger::GetInstance()->Trace();
    if (*m_device)
    {
        return m_device;
    }

    float queuePriority = 0.f;

    std::set<uint32_t> uniqueQueueFamilyIndices {m_graphicsQueueIndex, m_presentQueueIndex};
    std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos {};
    for (const auto queueIndex : uniqueQueueFamilyIndices)
    {
        deviceQueueCreateInfos.emplace_back(vk::DeviceQueueCreateInfo {{}, queueIndex, 1, &queuePriority});
    }

    vk::DeviceCreateInfo deviceCreateInfo({}, deviceQueueCreateInfos, {}, m_enableDeviceExtensionNames, {});
    m_device = vk::raii::Device(m_physicalDevice, deviceCreateInfo);

    return m_device;
}

std::pair<vk::Queue, vk::Queue> Device::InitQueues() noexcept
{
    Logger::GetInstance()->Trace();
    if (*m_graphicsQueue && *m_presentQueue)
    {
        return {m_graphicsQueue, m_presentQueue};
    }

    m_graphicsQueue = vk::raii::Queue(m_device, m_graphicsQueueIndex, 0);
    m_presentQueue  = vk::raii::Queue(m_device, m_presentQueueIndex, 0);

    return {m_graphicsQueue, m_presentQueue};
}

vk::CommandPool Device::InitCommandPool() noexcept
{
    if (*m_commandPool)
    {
        return m_commandPool;
    }

    m_commandPool =
        vk::raii::CommandPool(m_device, {{vk::CommandPoolCreateFlagBits::eResetCommandBuffer}, m_graphicsQueueIndex});

    return m_commandPool;
}

vk::DescriptorPool Device::InitDescriptorPool() noexcept
{
    if (*m_descriptorPool)
    {
        return m_descriptorPool;
    }

    // TODO
    std::vector<vk::DescriptorPoolSize> descriptorPoolSizes {
        {vk::DescriptorType::eCombinedImageSampler, JELLY_MAX_FRAMES * 100},
        {vk::DescriptorType::eUniformBuffer,        JELLY_MAX_FRAMES * 100}
    };

    m_descriptorPool = vk::raii::DescriptorPool(
        m_device,
        vk::DescriptorPoolCreateInfo {
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, JELLY_MAX_FRAMES * 100, descriptorPoolSizes
        }
    );

    return m_descriptorPool;
}

void Device::InitPipelineCache()
{
    m_pipelineCache = std::make_unique<PipelineCache>();
}

void Device::CreateDebugUtilsMessengerEXT() noexcept
{
    m_debugUtilsMessengerEXT = vk::raii::DebugUtilsMessengerEXT(
        m_instance,
        vk::DebugUtilsMessengerCreateInfoEXT {
            {},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            &DebugCallback
        }
    );
}

std::tuple<bool, uint32_t, uint32_t> Device::IsDeviceSuitable(
    const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::SurfaceKHR& surface
) const noexcept
{
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    std::optional<uint32_t> optPresentIndex {}, optGraphicsIndex {};
    for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i)
    {
        optPresentIndex = physicalDevice.getSurfaceSupportKHR(i, surface) ? i : optPresentIndex;

        if ((vk::QueueFlagBits::eGraphics & queueFamilyProperties[i].queueFlags)
            && (vk::QueueFlagBits::eTransfer & queueFamilyProperties[i].queueFlags))
        {
            optGraphicsIndex = i;
        }

        if (optPresentIndex && optGraphicsIndex)
        {
            break;
        }
    }

    auto supportSurface = !physicalDevice.getSurfacePresentModesKHR(surface).empty()
        && !physicalDevice.getSurfaceFormatsKHR(surface).empty();

    return std::forward_as_tuple(
        optPresentIndex && optGraphicsIndex && supportSurface, optGraphicsIndex.value_or(0), optPresentIndex.value_or(0)
    );
}
