#include "InteractorGLFW.h"
#include "WindowGLFW.h"
#include <GLFW/glfw3.h>
#include <any>

using namespace Jelly;

void InteractorGLFW::Start() const noexcept
{
    auto glfwWindow = std::any_cast<GLFWwindow*>(m_window->GetNativeWindow());
    while (!glfwWindowShouldClose(glfwWindow))
    {
        glfwPollEvents();
    }
}
