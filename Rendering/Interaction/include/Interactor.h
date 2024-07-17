/**
 * @file Interactor.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <array>
#include <memory>

namespace Jelly {
class Window;
class InteractorStyle;

class JELLY_EXPORT Interactor
    : public Object
    , public std::enable_shared_from_this<Interactor>
{
public:
    virtual void Start() noexcept = 0;

    void SetWindow(std::shared_ptr<Window> window) noexcept;
    const std::shared_ptr<Window>& GetWindow() const noexcept;

    void SetInteractorStyle(std::shared_ptr<InteractorStyle> interactorStyle) noexcept;

    const std::array<int, 2>& GetMousePosition() const noexcept;

protected:
    std::shared_ptr<Window> m_window {};
    std::shared_ptr<InteractorStyle> m_interactorStyle {};

    std::array<int, 2> m_mousePosition {};
};

} // namespace Jelly
