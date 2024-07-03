/**
 * @file WindowGLFW.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Window.h"

struct GLFWwindow;

namespace Jelly {
class JELLY_EXPORT WindowGLFW : public Window
{
protected:
    void InitSurface() noexcept override;
};
} // namespace Jelly
