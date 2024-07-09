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
#include <array>
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Actor;
class Device;
class Window;
class Viewer;

class JELLY_EXPORT Renderer : public Object
{
public:
    void Render(const vk::raii::CommandBuffer& commandBuffer) noexcept;

    void AddActor(std::shared_ptr<Actor> actor);

    void SetDevice(std::shared_ptr<Device> device) noexcept;
    void SetViewer(std::shared_ptr<Viewer> viewer) noexcept;
    void SetViewport(const std::array<double, 4>& viewport);
    void SetBackground(const std::array<float, 4>& background);

    std::shared_ptr<Camera> GetCamera() const noexcept;

    std::array<double, 6> GetVisibleActorBounds() const noexcept;

    bool IsInViewport(const std::array<int, 2>& position) const noexcept;

private:
    std::shared_ptr<Device> m_device {};
    std::weak_ptr<Viewer> m_viewer {};
    std::shared_ptr<Camera> m_camera {std::make_shared<Camera>()};
    std::vector<std::shared_ptr<Actor>> m_actors {};

    std::array<double, 4> m_viewport {0., 0., 1., 1.}; // 起始位置以及宽高
    std::array<float, 4> m_background {.3f, .2f, .1f, 1.f};
};
} // namespace Jelly
