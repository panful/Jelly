/**
 * @file InteractorGLFW.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Interactor.h"

struct GLFWwindow;

namespace Jelly {
class JELLY_EXPORT InteractorGLFW : public Interactor
{
public:
    void Start() noexcept override;

private:
    void SetWindowResized(uint32_t width, uint32_t height) noexcept;

private:
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) noexcept;
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept;
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) noexcept;
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
    static void WindowSizeCallback(GLFWwindow* window, int width, int height) noexcept;

private:
    bool m_windowResized {false};
    uint32_t m_windowWidth {};
    uint32_t m_windowHeight {};
};
} // namespace Jelly
