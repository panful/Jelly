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
/// @brief 右键旋转、左键移动、滚轮缩放
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
    std::array<int, 2> m_lastMousePosition {};
    double m_zoomFactor {0.1};
};
} // namespace Jelly
