/**
 * @file Window.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#pragma warning(disable : 4251)

#include "GlobalExport.h"
#include "Object.h"
#include <cstdint>
#include <string>
#include <string_view>

namespace Jelly {
class JELLY_EXPORT Window : public Object
{
public:
    void SetSize(const uint32_t width, const uint32_t height) noexcept;
    void SetTitle(const std::string_view title) noexcept;

private:
    uint32_t m_width {800};
    uint32_t m_height {600};
    std::string m_title {"Jelly"};
};
} // namespace Jelly
