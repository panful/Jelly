/**
 * @file RenderPass.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "DepthImageData.h"
#include "ImageData.h"
#include "Object.h"
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;

class JELLY_EXPORT RenderPass : public Object
{
public:
    RenderPass(std::shared_ptr<Device> device, const vk::Extent2D& extent);

    void Resize(const vk::Extent2D& extent) noexcept;

    const vk::raii::RenderPass& GetRenderPass() const noexcept;

    const std::vector<vk::raii::Framebuffer>& GetFramebuffers() const noexcept;

    std::vector<vk::ImageView> GetColorImageViews() const noexcept;

private:
    vk::Format m_colorFormat {vk::Format::eB8G8R8A8Unorm};
    vk::Format m_depthFormat {vk::Format::eD16Unorm};
    uint32_t m_maximumOfFrames {JELLY_MAX_FRAMES};

    std::shared_ptr<Device> m_device {};
    vk::raii::RenderPass m_renderPass {nullptr};
    std::vector<std::unique_ptr<ImageData>> m_colorImageDatas {};
    std::unique_ptr<DepthImageData> m_depthImageData {nullptr};
    std::vector<vk::raii::Framebuffer> m_framebuffers {};
};
} // namespace Jelly
