/**
 * @file Camera.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <array>
#include <atomic>
#include <cstdint>

namespace Jelly {
enum class CameraType : uint8_t
{
    Orthographic,
    Perspective,
};

class JELLY_EXPORT Camera : public Object
{
public:
    const std::array<float, 16>& GetViewMatrix() noexcept;
    const std::array<float, 16>& GetProjectMatrix() noexcept;
    const std::array<float, 3>& GetViewUp() const noexcept;
    const std::array<float, 3>& GetEyePos() const noexcept;
    const std::array<float, 3>& GetFocalPos() const noexcept;

    void SetAspectRatio(float aspectRatio) noexcept;

    void SetCameraType(CameraType cameraType) noexcept;

private:
    CameraType m_cameraType {CameraType::Orthographic};
    std::atomic_bool m_needUpdateView {true};
    std::atomic_bool m_needUpdateProject {true};

    float m_aspectRatio {1.f};

    std::array<float, 16> m_viewMatrix {};
    std::array<float, 16> m_projectMatrix {};

    std::array<float, 3> m_viewUp {0.f, -1.f, 0.f};
    std::array<float, 3> m_eyePos {0.f, 0.f, 0.f};
    std::array<float, 3> m_focalPos {0.f, 0.f, 1.f};
};
} // namespace Jelly
