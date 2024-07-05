/**
 * @file Window.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include "SwapChainData.h"
#include <any>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Renderer;
class Viewer;
class Pipeline;

class JELLY_EXPORT Window : public Object
{
public:
    Window() noexcept;
    ~Window() noexcept override;

    void Render() noexcept;

    void AddRenderer(std::shared_ptr<Renderer> renderer);

    void SetSize(const uint32_t width, const uint32_t height) noexcept;
    void SetTitle(const std::string_view title) noexcept;

    vk::Extent2D GetSize() const noexcept;
    std::any GetNativeWindow() const noexcept;
    std::shared_ptr<Device> GetDevice() const noexcept;

protected:
    virtual void InitSurface() noexcept = 0;

private:
    void PreRender() noexcept;
    void PostRender() noexcept;
    void Present() const noexcept;

    void InitWindow() noexcept;
    void InitSwapChain() noexcept;
    void InitRenderPass() noexcept;
    void InitFramebuffers() noexcept;
    void InitSyncObjects() noexcept;
    void InitCommandBuffers() noexcept;
    void InitSampler() noexcept;
    void InitPipeline() noexcept;
    void InitViewer() noexcept;
    void InitDescriptorSets() noexcept;
    void UpdateDescriptorSets() noexcept;

protected:
    uint32_t m_width {800};
    uint32_t m_height {600};
    std::any m_window {};
    std::string m_title {"Jelly"};
    std::shared_ptr<Device> m_device {};
    vk::raii::SurfaceKHR m_surface {nullptr};

private:
    uint32_t m_numberOfFrames {JELLY_MAX_FRAMES};
    uint32_t m_currentFrameIndex {0};
    uint32_t m_currentImageIndex {0};

    std::unique_ptr<Viewer> m_viewer {};

    SwapChainData m_swapChainData {nullptr};
    vk::raii::RenderPass m_renderPass {nullptr};
    std::vector<vk::raii::Framebuffer> m_framebuffers {};

    std::unique_ptr<Pipeline> m_pipeline {};
    vk::raii::Sampler m_sampler {nullptr};

    vk::raii::CommandBuffers m_commandBuffers {nullptr};
    vk::raii::DescriptorSets m_descriptorSets {nullptr};

    std::vector<vk::raii::Fence> m_inFlightFences {};
    std::vector<vk::raii::Semaphore> m_renderFinishedSemaphores {};
    std::vector<vk::raii::Semaphore> m_imageAcquiredSemaphores {};
};
} // namespace Jelly
