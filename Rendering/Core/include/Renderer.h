/**
 * @file Renderer.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Camera.h"
#include "Object.h"
#include "Pipeline.h"
#include <array>
#include <memory>
#include <mutex>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Actor;
class Window;
class Viewer;

class JELLY_EXPORT Renderer : public Object
{
public:
    ~Renderer() noexcept override
    {
        std::cout << __func__ << std::endl;
    }
    
    void Render(const vk::raii::CommandBuffer& commandBuffer) noexcept;

    void AddActor(std::shared_ptr<Actor> actor);

    const std::vector<std::shared_ptr<Actor>>& GetAllActors() const noexcept;

    void SetViewport(const std::array<double, 4>& viewport);

    const std::array<double, 4>& GetViewport() const noexcept;

    void SetBackground(const std::array<float, 4>& background);

    void SetBackground2(const std::array<float, 4>& background);

    void SetEnableGradientBackground(bool enable) noexcept;

    void SetViewer(std::weak_ptr<Viewer> viewer) noexcept;

    std::shared_ptr<Viewer> GetViewer() noexcept;

    /// @brief 重置相机，使所有Actor居中显示
    void ResetCamera() const noexcept;

    /// @brief 根据包围盒以及相机位置重置相机的裁剪面
    void ResetCameraClipRange() const noexcept;

    const std::shared_ptr<Camera>& GetCamera() const noexcept;

    std::array<double, 6> GetVisibleActorBounds() const noexcept;

    bool IsInViewport(const std::array<int, 2>& position) const noexcept;

    /// @brief 世界坐标转换为窗口坐标
    /// @param worldPoint
    /// @return 左上角为窗口原点
    std::array<int, 2> WorldToDisplay(const std::array<double, 3>& worldPoint) const noexcept;

    /// @brief 窗口坐标转换为世界坐标
    /// @param displayPoint
    /// @return
    std::array<double, 3> DisplayToWorld(const std::array<int, 2>& displayPoint) const noexcept;

private:
    void CreateBackgroundPipeline(std::shared_ptr<Viewer>& viewer);

private:
    std::once_flag m_createBackgroundPipeline {};
    std::weak_ptr<Viewer> m_viewer {};
    std::shared_ptr<Camera> m_camera {std::make_shared<Camera>()};
    std::vector<std::shared_ptr<Actor>> m_actors {};

    std::array<double, 4> m_viewport {0., 0., 1., 1.}; // 起始位置以及宽高

    bool m_enableGradientBackground {false};
    std::unique_ptr<Pipeline> m_backgroundPipeline {};
    std::array<float, 4> m_background {.3f, .2f, .1f, 1.f};
    std::array<float, 4> m_background2 {.3f, .2f, .1f, 1.f};
};
} // namespace Jelly
