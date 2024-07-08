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

namespace Jelly {
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
};
} // namespace Jelly
