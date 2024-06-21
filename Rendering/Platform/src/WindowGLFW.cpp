#include "WindowGLFW.h"
#include "Device.h"
#include "GLFWHelper.h"
#include <GLFW/glfw3.h>
#include <mutex>

using namespace Jelly;

void WindowGLFW::Render() noexcept
{
    static std::once_flag flag {};
    std::call_once(flag, [this]() {
        m_window = GLFWHelper::GetInstance()->CreateWindow(m_width, m_height, m_title.c_str());

        VkSurfaceKHR surface {nullptr};
        glfwCreateWindowSurface(static_cast<VkInstance>(m_device->InitInstance()), m_window, nullptr, &surface);
        m_surface = vk::raii::SurfaceKHR(m_device->GetInstance(), surface);

        m_device->PickPhysicalDevice(m_surface);
        m_device->InitDevice();
        m_device->InitQueues();
    });
}

void* WindowGLFW::GetNativeWindow() const noexcept
{
    return m_window;
}
