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
#pragma warning(disable : 4251)

#include "Object.h"
#include <memory>

namespace Jelly {
class Window;

class JELLY_EXPORT Interactor : public Object
{
public:
    virtual void Start() const noexcept = 0;

    void SetWindow(std::shared_ptr<Window> window) noexcept;

protected:
    std::shared_ptr<Window> m_window {};
};

} // namespace Jelly
