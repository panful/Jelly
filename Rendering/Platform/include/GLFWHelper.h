/**
 * @file GLFWHelper.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <cstdint>
#include <mutex>

struct GLFWwindow;

namespace Jelly {
class GLFWHelper
{
private:
    GLFWHelper() noexcept;
    ~GLFWHelper() noexcept;

    GLFWHelper(const GLFWHelper&)            = delete;
    GLFWHelper& operator=(const GLFWHelper&) = delete;

public:
    static GLFWHelper* GetInstance() noexcept;
    GLFWwindow* CreateWindow(uint32_t width, uint32_t height, const char* title) const noexcept;
    void DestroyWindow(GLFWwindow* window);

private:
    mutable std::mutex m_createMutex {};
};
} // namespace Jelly
