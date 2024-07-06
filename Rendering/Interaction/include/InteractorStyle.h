/**
 * @file InteractorStyle.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"

namespace Jelly {
class JELLY_EXPORT InteractorStyle : public Object
{
public:
    virtual void MouseMoveEvent();

    virtual void RightButtonPressEvent();
    virtual void RightButtonReleaseEvent();

    virtual void LeftButtonPressEvent();
    virtual void LeftButtonReleaseEvent();

    virtual void MiddleButtonPressEvent();
    virtual void MiddleButtonReleaseEvent();

    virtual void MouseWheelForwardEvent();
    virtual void MouseWheelBackwardEvent();

    virtual void CharEvent();

    virtual void KeyPressEvent();
    virtual void KeyReleaseEvent();

    virtual void EnterEvent();
    virtual void LeaveEvent();
};
} // namespace Jelly
