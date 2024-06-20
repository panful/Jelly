#include "GLFWHelper.h"
#include "Logger.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <format>

using namespace Jelly;

GLFWHelper::GLFWHelper() noexcept
{
    if (!glfwInit())
    {
        Logger::GetInstance()->Error("Failed to init glfw");
    }

    glfwSetErrorCallback([](int error, const char* msg) {
        Logger::GetInstance()->Error(std::format("GLFW [{}] : {}", error, msg));
    });

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

GLFWHelper::~GLFWHelper() noexcept
{
    Logger::GetInstance()->Trace("glfw terminate");
    glfwTerminate();
}

GLFWHelper* GLFWHelper::GetInstance() noexcept
{
    static GLFWHelper helper {};
    return &helper;
}

GLFWwindow* GLFWHelper::CreateWindow(uint32_t width, uint32_t height, const char* title) const noexcept
{
    std::lock_guard lk(m_createMutex);
    if (auto window = glfwCreateWindow(width, height, title, nullptr, nullptr))
    {
        return window;
    }

    Logger::GetInstance()->Error("Failed to create glfw window");
    return nullptr;
}

void GLFWHelper::DestroyWindow(GLFWwindow* window)
{
    glfwDestroyWindow(window);
}
