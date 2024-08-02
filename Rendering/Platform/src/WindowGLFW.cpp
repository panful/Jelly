#include "WindowGLFW.h"
#include "Device.h"
#include "GLFWHelper.h"
#include "Viewer.h"
#include <GLFW/glfw3.h>

using namespace Jelly;

void WindowGLFW::InitSurface() noexcept
{
    auto window = GLFWHelper::GetInstance()->CreateWindow(m_extent.width, m_extent.height, m_title.c_str());

    VkSurfaceKHR surface {nullptr};
    glfwCreateWindowSurface(static_cast<VkInstance>(Device::Get()->InitInstance()), window, nullptr, &surface);

    m_surface = vk::raii::SurfaceKHR(Device::Get()->GetInstance(), surface);
    m_window  = window;
}
