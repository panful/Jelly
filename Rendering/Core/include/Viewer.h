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
    const std::unique_ptr<RenderPass>& GetRenderPass() const noexcept;
    std::vector<vk::ImageView> GetColorImageViews() const noexcept;
    uint32_t GetCurrentFrameIndex() const noexcept;
    uint32_t GetMaximumOfFrames() const noexcept;

private:
    vk::Extent2D m_extent {};
    uint32_t m_currentFrameIndex {0};
    uint32_t m_maximumOfFrames {JELLY_MAX_FRAMES};

    std::shared_ptr<Device> m_device {};
    std::unique_ptr<RenderPass> m_renderPass {};
    std::vector<std::shared_ptr<Renderer>> m_renderers {};
};
} // namespace Jelly
