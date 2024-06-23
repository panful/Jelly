#include "InteractorGLFW.h"
#include "WindowGLFW.h"
#include <GLFW/glfw3.h>

using namespace Jelly;

void InteractorGLFW::Start() const noexcept
{
    auto window = static_cast<GLFWwindow*>(std::dynamic_pointer_cast<WindowGLFW>(m_window)->GetNativeWindow());
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}
