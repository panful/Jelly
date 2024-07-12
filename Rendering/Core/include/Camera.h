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
#include <numbers>

namespace Jelly {
enum class CameraType : uint8_t
{
    Orthographic,
    Perspective,
};

class JELLY_EXPORT Camera : public Object
{
public:
    /// @brief 获取观察矩阵
    /// @return 列优先，可直接赋值给 GLSL 中的 mat4
    const std::array<float, 16>& GetViewMatrix() noexcept;

    /// @brief 获取投影矩阵
    /// @return 列优先，可直接赋值给 GLSL 中的 mat4
    const std::array<float, 16>& GetProjectMatrix() noexcept;

    /// @brief 获取相机向上的方向
    /// @return
    const std::array<double, 3>& GetViewUp() const noexcept;

    void SetViewUp(const std::array<double, 3>& viewUp) noexcept;

    /// @brief 获取相机的位置
    /// @return
    const std::array<double, 3>& GetEyePos() const noexcept;

    void SetEyePos(const std::array<double, 3>& eyePos) noexcept;

    /// @brief 获取相机焦点的位置
    /// @return
    const std::array<double, 3>& GetFocalPos() const noexcept;

    void SetFocalPos(const std::array<double, 3>& focalPos) noexcept;

    /// @brief 俯仰
    /// @details 绕相机的X轴旋转，即相机投影方向和朝上方向的叉乘，以相机位置为旋转中心
    /// @param angle 旋转角度，单位是弧度
    void Pitch(double angle) noexcept;

    /// @brief 偏航
    /// @details 绕相机的Y轴旋转，即相机朝上的方向，以相机位置为旋转中心
    /// @param angle 旋转角度，单位是弧度
    void Yaw(double angle) noexcept;

    /// @brief 横滚
    /// @details 绕相机的Z轴旋转，即相机投影方向，以相机位置为旋转中心
    /// @param angle 旋转角度，单位是弧度
    void Roll(double angle) noexcept;

    /// @brief 方位角
    /// @details 绕相机的Y轴旋转，以焦点为旋转中心
    /// @param angle 旋转角度，单位是弧度
    void Azimuth(double angle) noexcept;

    /// @brief 高度角
    /// @details 绕相机的X轴旋转，以焦点为旋转中心
    /// @param angle 旋转角度，单位是弧度
    void Elevation(double angle) noexcept;

    /// @brief 设置相机的视角角度
    /// @param viewAngle 单位是弧度
    void SetViewAngle(double viewAngle) noexcept;

    double GetViewAngle() const noexcept;

    /// @brief 设置正交投影的缩放比例
    /// @param orthographicScale
    void SetOrthographicScale(double orthographicScale) noexcept;

    double GetOrthographicScale() const noexcept;

    /// @brief 获取投影平面的法线，通常和投影方向相反，即相机焦点指向相机位置的方向
    /// @return 归一化的向量
    std::array<double, 3> GetViewPlaneNormal() const noexcept;

    /// @brief 获取投影的方向，即相机指向焦点的方向
    /// @return 归一化的向量
    std::array<double, 3> GetProjectionDirection() const noexcept;

    /// @brief 设置裁剪平面到相机的距离
    /// @param clipRange 第一个值为近裁剪平面，第二个值为远裁剪平面
    void SetClipRange(const std::array<double, 2>& clipRange) noexcept;

    const std::array<double, 2>& GetClipRange() const noexcept;

    /// @brief 设置视口的宽高比
    /// @param aspectRatio
    void SetAspectRatio(double aspectRatio) noexcept;

    void SetCameraType(CameraType cameraType) noexcept;

    CameraType GetCameraType() const noexcept;

    /// @brief NDC坐标转换为世界坐标
    /// @param ndcPoint Normalized Device Coordinates
    /// @note Vulkan的NDC坐标范围是 X:[-1,1], Y:[-1,1], Z:[0,1]
    /// @return
    std::array<double, 3> NDCToWorld(const std::array<double, 3>& ndcPoint) const noexcept;

    /// @brief 世界坐标转换为观察坐标
    /// @param worldPoint
    /// @return
    std::array<double, 3> WorldToView(const std::array<double, 3>& worldPoint) const noexcept;

private:
    void Update();

private:
    CameraType m_cameraType {CameraType::Orthographic};
    std::atomic_bool m_needUpdate {true};

    double m_aspectRatio {1.};
    double m_viewAngle {std::numbers::pi_v<double> / 180. * 30.}; // 视角角度默认30度
    double m_orthographicScale {1.};

    std::array<double, 4> m_viewRange {-1., 1., -1., 1.};         // 视野宽高范围
    std::array<double, 2> m_clipRange {0., 1.};                   // 深度裁剪范围

    std::array<float, 16> m_viewMatrix {};
    std::array<float, 16> m_projectMatrix {};

    std::array<double, 3> m_viewUp {0.f, -1.f, 0.f};
    std::array<double, 3> m_eyePos {0.f, 0.f, 0.f};
    std::array<double, 3> m_focalPos {0.f, 0.f, 1.f};
};
} // namespace Jelly
