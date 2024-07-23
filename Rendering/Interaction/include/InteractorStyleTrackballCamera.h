/**
 * @file InteractorStyleTrackballCamera.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "InteractorStyle.h"
#include <array>

namespace Jelly {
/// @brief 右键旋转、左键移动、滚轮缩放（以鼠标所在位置为缩放中心）、中键缩放（以窗口中心为缩放中心）
class JELLY_EXPORT InteractorStyleTrackballCamera : public InteractorStyle
{
protected:
    void MouseMoveEvent() override;

    void RightButtonPressEvent() override;
    void RightButtonReleaseEvent() override;

    void LeftButtonPressEvent() override;
    void LeftButtonReleaseEvent() override;

    void MiddleButtonPressEvent() override;
    void MiddleButtonReleaseEvent() override;

    void MouseWheelForwardEvent() override;
    void MouseWheelBackwardEvent() override;

protected:

    /// @brief 以鼠标所在位置为中心进行缩放
    /// @param factor 小于1缩小，大于1放大，小于等于0无效
    void ZoomByMousePoint(double factor);

protected:
    std::array<int, 2> m_lastMousePosition {};
    double m_zoomFactor {0.1};
};
} // namespace Jelly
