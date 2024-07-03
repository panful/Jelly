/**
 * @file Viewer.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "DepthImageData.h"
#include "ImageData.h"
#include "Object.h"
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Renderer;

class JELLY_EXPORT Viewer : public Object
{
public:
    void Init(std::shared_ptr<Device> device, const vk::Extent2D& extent);

    void Render(const vk::raii::CommandBuffer& commandBuffer);

    void AddRenderer(std::shared_ptr<Renderer> renderer);

    std::vector<vk::ImageView> GetColorImageViews() const noexcept;

private:
    uint32_t m_currentImageIndex {0};
    uint32_t m_numberOfImages {JELLY_MAX_FRAMES};
    vk::Extent2D m_extent {};
    vk::Format m_colorFormat {vk::Format::eB8G8R8A8Unorm};
    vk::Format m_depthFormat {vk::Format::eD16Unorm};

    std::shared_ptr<Device> m_device {};
    vk::raii::RenderPass m_renderPass {nullptr};

    std::vector<ImageData> m_colorImageDatas {};
    DepthImageData m_depthImageData {nullptr};

    std::vector<vk::raii::Framebuffer> m_framebuffers {};
    std::vector<std::shared_ptr<Renderer>> m_renderers {};
};
} // namespace Jelly
