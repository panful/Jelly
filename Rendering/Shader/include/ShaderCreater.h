/**
 * @file ShaderCreater.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <array>
#include <cstdint>
#include <string>

namespace Jelly {
class JELLY_EXPORT ShaderCreater : public Object
{
public:
    ShaderCreater();

    std::string GetVertexShaderCode() const noexcept;
    std::string GetFragmentShaderCode() const noexcept;

    void AddPointColor(uint32_t location);
    void RemovePointColor();
    void SetFragColor(const std::array<double, 3>& color);

private:
    /// @brief 将str中start到end之间的字符全部使用replacement替换
    /// @details start和end保留
    /// @param str
    /// @param start
    /// @param end
    /// @param replacement
    static void
    ReplaceValue(std::string& str, const std::string& start, const std::string& end, const std::string& replacement);

    /// @brief 将str中的oldStr使用newStr替换
    /// @param str
    /// @param oldStr
    /// @param newStr
    static void ReplaceValue(std::string& str, const std::string& oldStr, const std::string& newStr);

private:
    std::string m_vertexShaderCode {};
    std::string m_fragmentShaderCode {};
};
} // namespace Jelly
