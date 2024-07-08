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
#include <cstdint>
#include <memory>

namespace Jelly {
class Interactor;
class Renderer;

enum class InteractorState : uint8_t
{
    None,
    Rotate, // 旋转
    Pan,    // 平移
    Zoom,   // 缩放
};

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
    void FindPokedRenderer();

protected:
    InteractorState m_state {InteractorState::None};

    std::weak_ptr<Interactor> m_interactor {};
    std::weak_ptr<Renderer> m_currentRenderer {};
};
} // namespace Jelly
