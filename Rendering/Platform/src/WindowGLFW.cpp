#include "WindowGLFW.h"
#include "GLFWHelper.h"
#include <mutex>

using namespace Jelly;

void WindowGLFW::Render() const noexcept
{
    static std::once_flag flag {};
    std::call_once(flag, [this]() {
        m_window = GLFWHelper::GetInstance()->CreateWindow(m_width, m_height, m_title.c_str());
    });
}
