#include "Window.h"
#include "Device.h"

using namespace Jelly;

Window::Window() noexcept
    : m_device(std::make_shared<Device>())
{
}

void Window::SetSize(const uint32_t width, const uint32_t height) noexcept
{
    m_width  = width;
    m_height = height;
}

void Window::SetTitle(const std::string_view title) noexcept
{
    m_title = title;
}
