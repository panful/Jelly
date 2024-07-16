/**
 * @file SwapChainData.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Device.h"

namespace Jelly {
class SwapChainData
{
public:
    SwapChainData(std::nullptr_t);

    SwapChainData(
        std::shared_ptr<Device> device,
        const vk::raii::SurfaceKHR& surface,
        const vk::Extent2D& extent,
        const vk::raii::SwapchainKHR* pOldSwapchain,
        vk::ImageUsageFlags usage
    );

    SwapChainData(SwapChainData&&) noexcept = default;

    SwapChainData& operator=(SwapChainData&&) noexcept = default;

    SwapChainData(const SwapChainData&) = delete;

    SwapChainData& operator=(const SwapChainData&) = delete;

    ~SwapChainData() noexcept = default;

    uint32_t GetNumberOfImages() const noexcept;
    vk::Format GetColorFormat() const noexcept;
    vk::Image GetImage(uint32_t index) const noexcept;
    const vk::Extent2D& GetExtent() const noexcept;
    const vk::raii::ImageView& GetImageView(uint32_t index) const noexcept;
    const vk::raii::SwapchainKHR& GetSwapChain() const noexcept;

private:
    static vk::PresentModeKHR PickPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
    static vk::SurfaceFormatKHR PickSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);

private:
    std::shared_ptr<Device> m_device {};
    vk::Format m_colorFormat {};
    vk::raii::SwapchainKHR m_swapChain {nullptr};
    std::vector<vk::Image> m_images {};
    std::vector<vk::raii::ImageView> m_imageViews {};
    vk::Extent2D m_swapchainExtent {};
    uint32_t m_numberOfImages {};
};
} // namespace Jelly
