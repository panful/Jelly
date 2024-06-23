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
#pragma warning(disable : 4251)

#include "Object.h"
#include "SwapChainData.h"
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Renderer;

class JELLY_EXPORT Window : public Object
{
public:
    Window() noexcept;
    ~Window() noexcept override;

    void AddRenderer(std::shared_ptr<Renderer> renderer);

    virtual void Render() noexcept = 0;

    virtual void* GetNativeWindow() const noexcept = 0;

    void SetSize(const uint32_t width, const uint32_t height) noexcept;
    void SetTitle(const std::string_view title) noexcept;

protected:
    void InitWindow() noexcept;
    void PreRender() noexcept;
    void PostRender() noexcept;

private:
    void InitSwapChain() noexcept;
    void InitRenderPass() noexcept;
    void InitFramebuffers() noexcept;
    void InitSyncObjects() noexcept;
    void InitCommandPool() noexcept;
    void InitCommandBuffers() noexcept;

protected:
    uint32_t m_width {800};
    uint32_t m_height {600};
    uint32_t m_numberOfFrames {3};
    uint32_t m_currentFrameIndex {0};
    uint32_t m_currentImageIndex {0};

    std::string m_title {"Jelly"};

    std::shared_ptr<Device> m_device {};
    vk::raii::CommandPool m_commandPool {nullptr};
    vk::raii::SurfaceKHR m_surface {nullptr};
    SwapChainData m_swapChainData {nullptr};
    vk::raii::RenderPass m_renderPass {nullptr};
    vk::raii::CommandBuffers m_commandBuffers {nullptr};
    std::vector<vk::raii::Framebuffer> m_framebuffers {};
    std::vector<vk::raii::Fence> m_inFlightFences {};
    std::vector<vk::raii::Semaphore> m_renderFinishedSemaphores {};
    std::vector<vk::raii::Semaphore> m_imageAcquiredSemaphores {};

    std::vector<std::shared_ptr<Renderer>> m_renderers {};
};
} // namespace Jelly
