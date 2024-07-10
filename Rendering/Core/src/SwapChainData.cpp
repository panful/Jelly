#include "SwapChainData.h"
#include "Logger.h"
#include <algorithm>
#include <array>
#include <format>

using namespace Jelly;

SwapChainData::SwapChainData(std::nullptr_t)
{
}

SwapChainData::SwapChainData(
    std::shared_ptr<Device> device,
    const vk::raii::SurfaceKHR& surface,
    const vk::Extent2D& extent,
    const vk::raii::SwapchainKHR* pOldSwapchain,
    vk::ImageUsageFlags usage
)
    : m_device(std::move(device))
{
    vk::SurfaceFormatKHR surfaceFormat = PickSurfaceFormat(m_device->GetPhysicalDevice().getSurfaceFormatsKHR(surface));

    m_colorFormat = surfaceFormat.format;

    vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_device->GetPhysicalDevice().getSurfaceCapabilitiesKHR(surface);
    if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
    {
        // 如果表面大小未定义，则将大小设置为所请求图像的大小。
        m_swapchainExtent = vk::Extent2D {
            std::clamp(
                extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width
            ),
            std::clamp(
                extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height
            )
        };
    }
    else
    {
        // 如果定义了表面大小，则交换链大小必须匹配
        m_swapchainExtent = surfaceCapabilities.currentExtent;
    }

    if (m_swapchainExtent != extent)
    {
        Logger::GetInstance()->Warn(std::format(
            "Surface extent incompatible, need extent: ({}, {}), current surface extent: ({} ,{})",
            extent.width,
            extent.height,
            m_swapchainExtent.width,
            m_swapchainExtent.height
        ));
    }

    vk::SurfaceTransformFlagBitsKHR preTransform =
        (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
        ? vk::SurfaceTransformFlagBitsKHR::eIdentity
        : surfaceCapabilities.currentTransform;
    vk::CompositeAlphaFlagBitsKHR compositeAlpha =
        (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
        ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
        : (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
        ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
        : (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit)
        ? vk::CompositeAlphaFlagBitsKHR::eInherit
        : vk::CompositeAlphaFlagBitsKHR::eOpaque;

    m_numberOfImages = std::clamp(3u, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);

    vk::PresentModeKHR presentMode = PickPresentMode(m_device->GetPhysicalDevice().getSurfacePresentModesKHR(surface));
    vk::SwapchainCreateInfoKHR swapChainCreateInfo(
        {},
        surface,
        m_numberOfImages,
        m_colorFormat,
        surfaceFormat.colorSpace,
        m_swapchainExtent,
        1,
        usage,
        vk::SharingMode::eExclusive,
        {},
        preTransform,
        compositeAlpha,
        presentMode,
        true,
        pOldSwapchain ? **pOldSwapchain : nullptr
    );

    const auto graphicsQueueFamilyIndex = m_device->GetGraphicsQueueIndex();
    const auto presentQueueFamilyIndex  = m_device->GetPresentQueueIndex();

    if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
    {
        uint32_t queueFamilyIndices[2]            = {graphicsQueueFamilyIndex, presentQueueFamilyIndex};
        swapChainCreateInfo.imageSharingMode      = vk::SharingMode::eConcurrent;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices;
    }

    m_swapChain = vk::raii::SwapchainKHR(m_device->GetDevice(), swapChainCreateInfo);
    m_images    = m_swapChain.getImages();

    m_imageViews.reserve(m_images.size());
    vk::ImageViewCreateInfo imageViewCreateInfo(
        {}, {}, vk::ImageViewType::e2D, m_colorFormat, {}, {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
    );
    for (auto image : m_images)
    {
        imageViewCreateInfo.image = image;
        m_imageViews.emplace_back(m_device->GetDevice(), imageViewCreateInfo);
    }
}

vk::Format SwapChainData::GetColorFormat() const noexcept
{
    return m_colorFormat;
}

uint32_t SwapChainData::GetNumberOfImages() const noexcept
{
    return static_cast<uint32_t>(m_images.size());
}

vk::Image SwapChainData::GetImage(uint32_t index) const noexcept
{
    return m_images[index];
}

const vk::Extent2D& SwapChainData::GetExtent() const noexcept
{
    return m_swapchainExtent;
}

const vk::raii::ImageView& SwapChainData::GetImageView(uint32_t index) const noexcept
{
    return m_imageViews[index];
}

const vk::raii::SwapchainKHR& SwapChainData::GetSwapChain() const noexcept
{
    return m_swapChain;
}

vk::SurfaceFormatKHR SwapChainData::PickSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
{
    if (formats.empty())
    {
        Logger::GetInstance()->Error("The provided foramts is empty");
    }

    vk::SurfaceFormatKHR pickedFormat = formats[0];
    if (formats.size() == 1)
    {
        if (formats[0].format == vk::Format::eUndefined)
        {
            pickedFormat.format     = vk::Format::eB8G8R8A8Unorm;
            pickedFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        }
    }
    else
    {
        // 使用第一个符合条件的格式
        std::array requestedFormats {
            vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8Unorm, vk::Format::eR8G8B8Unorm
        };

        vk::ColorSpaceKHR requestedColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        for (auto requestedFormat : requestedFormats)
        {
            auto it = std::find_if(
                formats.begin(),
                formats.end(),
                [requestedFormat, requestedColorSpace](vk::SurfaceFormatKHR const& f) {
                    return (f.format == requestedFormat) && (f.colorSpace == requestedColorSpace);
                }
            );
            if (it != formats.end())
            {
                pickedFormat = *it;
                break;
            }
        }
    }

    if (pickedFormat.colorSpace != vk::ColorSpaceKHR::eSrgbNonlinear)
    {
        Logger::GetInstance()->Error("pickedFormat.colorSpace != vk::ColorSpaceKHR::eSrgbNonlinear");
    }

    return pickedFormat;
}

vk::PresentModeKHR SwapChainData::PickPresentMode(const std::vector<vk::PresentModeKHR>& presentModes)
{
    vk::PresentModeKHR pickedMode = vk::PresentModeKHR::eFifo;
    for (const auto& presentMode : presentModes)
    {
        if (presentMode == vk::PresentModeKHR::eMailbox)
        {
            pickedMode = presentMode;
            break;
        }

        if (presentMode == vk::PresentModeKHR::eImmediate)
        {
            pickedMode = presentMode;
        }
    }
    return pickedMode;
}
