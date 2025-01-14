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
    ~InteractorStyle() noexcept override
    {
        std::cout << __func__ << std::endl;
    }

    void SetInteractor(std::weak_ptr<Interactor> interactor) noexcept;

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

    /// @brief 推拉相机实现场景缩放
    /// @param renderer 相机所在的 Renderer
    /// @param factor 小于1缩小，大于1放大，小于等于0无效
    void Dolly(const std::shared_ptr<Renderer>& renderer, double factor) const noexcept;

    /// @brief 将相机从指定位置移动到另一个位置
    /// @param renderer
    /// @param from
    /// @param to
    void TranslateCamera(
        const std::shared_ptr<Renderer>& renderer, const std::array<double, 3>& from, const std::array<double, 3>& to
    ) const noexcept;

protected:
    InteractorState m_state {InteractorState::None};

    std::weak_ptr<Interactor> m_interactor {};
    std::weak_ptr<Renderer> m_currentRenderer {};
};
} // namespace Jelly
