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
#include "RenderPass.h"
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Renderer;

class JELLY_EXPORT Viewer
    : public Object
    , public std::enable_shared_from_this<Viewer>
{
public:
    void SetDevice(std::shared_ptr<Device> device);

    void Init(const vk::Extent2D& extent);

    void Render(const vk::raii::CommandBuffer& commandBuffer);

    void AddRenderer(std::shared_ptr<Renderer> renderer);

    void Resize(const vk::Extent2D& extent);

    const std::vector<std::shared_ptr<Renderer>>& GetAllRenderers() const noexcept;
    const vk::Extent2D& GetExtent() const noexcept;
    const vk::raii::RenderPass& GetRenderPass() const noexcept;
    std::vector<vk::ImageView> GetColorImageViews() const noexcept;
    uint32_t GetCurrentFrameIndex() const noexcept;
    uint32_t GetMaximumOfFrames() const noexcept;

private:
    uint32_t m_currentFrameIndex {0};
    uint32_t m_maximumOfFrames {JELLY_MAX_FRAMES};
    vk::Extent2D m_extent {};
    vk::Format m_colorFormat {vk::Format::eB8G8R8A8Unorm};
    vk::Format m_depthFormat {vk::Format::eD16Unorm};

    std::shared_ptr<Device> m_device {};
    std::unique_ptr<RenderPass> m_renderPass {};

    std::vector<std::unique_ptr<ImageData>> m_colorImageDatas {};
    std::unique_ptr<DepthImageData> m_depthImageData {nullptr};

    std::vector<vk::raii::Framebuffer> m_framebuffers {};
    std::vector<std::shared_ptr<Renderer>> m_renderers {};
};
} // namespace Jelly
