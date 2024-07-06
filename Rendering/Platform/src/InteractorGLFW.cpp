#include "InteractorGLFW.h"
#include "WindowGLFW.h"
#include <GLFW/glfw3.h>
#include <any>

using namespace Jelly;

void InteractorGLFW::Start() noexcept
{
    auto glfwWindow = std::any_cast<GLFWwindow*>(m_window->GetNativeWindow());

    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetCursorPosCallback(glfwWindow, CursorPosCallback);
    glfwSetKeyCallback(glfwWindow, KeyCallback);
    glfwSetScrollCallback(glfwWindow, ScrollCallback);
    glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
    glfwSetWindowSizeCallback(glfwWindow, WindowSizeCallback);

    if (!m_window->HasInitialized())
    {
        m_window->Render();
    }

    while (!glfwWindowShouldClose(glfwWindow))
    {
        glfwPollEvents();

        if (m_windowResized)
        {
            m_window->SetSize(m_windowWidth, m_windowHeight);
            m_window->Render();
            m_windowResized = false;
        }
    }
}

void InteractorGLFW::SetWindowResized(uint32_t width, uint32_t height) noexcept
{
    m_windowWidth   = width;
    m_windowHeight  = height;
    m_windowResized = true;
}

void InteractorGLFW::WindowSizeCallback(GLFWwindow* window, int width, int height) noexcept
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    if (auto pInstance = static_cast<InteractorGLFW*>(glfwGetWindowUserPointer(window)))
    {
        pInstance->SetWindowResized(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    }
}

void InteractorGLFW::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    if (auto pInstance = static_cast<InteractorGLFW*>(glfwGetWindowUserPointer(window)))
    {
    }
}

void InteractorGLFW::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept
{
    if (auto pInstance = static_cast<InteractorGLFW*>(glfwGetWindowUserPointer(window)))
    {
        switch (action)
        {
            case GLFW_PRESS:
            {
                switch (button)
                {
                    case GLFW_MOUSE_BUTTON_LEFT:
                        break;
                    case GLFW_MOUSE_BUTTON_MIDDLE:
                        break;
                    case GLFW_MOUSE_BUTTON_RIGHT:
                        break;
                }
                break;
            }
            case GLFW_RELEASE:
            {
                switch (button)
                {
                    case GLFW_MOUSE_BUTTON_LEFT:
                        break;
                    case GLFW_MOUSE_BUTTON_MIDDLE:
                        break;
                    case GLFW_MOUSE_BUTTON_RIGHT:
                        break;
                }
                break;
            }
        }
    }
}

void InteractorGLFW::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) noexcept
{
    if (auto pInstance = static_cast<InteractorGLFW*>(glfwGetWindowUserPointer(window)))
    {
        if (yoffset > 0.0)
        {
        }
        else if (yoffset < 0.0)
        {
        }
    }
}

void InteractorGLFW::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    if (auto pInstance = static_cast<InteractorGLFW*>(glfwGetWindowUserPointer(window)))
    {
        switch (action)
        {
            case GLFW_PRESS:
                break;
            case GLFW_RELEASE:
                break;
            case GLFW_REPEAT:
                break;
        }

        switch (key)
        {
            case GLFW_KEY_0:
            case GLFW_KEY_1:
            case GLFW_KEY_2:
            case GLFW_KEY_3:
            case GLFW_KEY_4:
            case GLFW_KEY_5:
            case GLFW_KEY_6:
            case GLFW_KEY_7:
            case GLFW_KEY_8:
            case GLFW_KEY_9:
                break;
            case GLFW_KEY_A:
            case GLFW_KEY_B:
            case GLFW_KEY_C:
            case GLFW_KEY_D:
            case GLFW_KEY_E:
            case GLFW_KEY_F:
            case GLFW_KEY_G:
            case GLFW_KEY_H:
            case GLFW_KEY_I:
            case GLFW_KEY_J:
            case GLFW_KEY_K:
            case GLFW_KEY_L:
            case GLFW_KEY_M:
            case GLFW_KEY_N:
            case GLFW_KEY_O:
            case GLFW_KEY_P:
            case GLFW_KEY_Q:
            case GLFW_KEY_R:
            case GLFW_KEY_S:
            case GLFW_KEY_T:
            case GLFW_KEY_U:
            case GLFW_KEY_V:
            case GLFW_KEY_W:
            case GLFW_KEY_X:
            case GLFW_KEY_Y:
            case GLFW_KEY_Z:
                // XXX mods == GLFW_MOD_CAPS_LOCK ? key : key + 32;
                break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_LEFT:
            case GLFW_KEY_DOWN:
            case GLFW_KEY_UP:
                break;
        }

        switch (mods)
        {
            case GLFW_MOD_SHIFT:
                break;
            case GLFW_MOD_CONTROL:
                break;
            case GLFW_MOD_ALT:
                break;
            case GLFW_MOD_CAPS_LOCK:
                break;
            case GLFW_MOD_NUM_LOCK:
                break;
        }
    }
}
