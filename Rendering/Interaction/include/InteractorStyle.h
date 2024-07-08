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
#include <memory>

namespace Jelly {
class Interactor;

class JELLY_EXPORT InteractorStyle : public Object
{
public:
    void SetInteractor(std::shared_ptr<Interactor> interactor) noexcept;

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

protected:
    std::weak_ptr<Interactor> m_interactor {};
};
} // namespace Jelly
