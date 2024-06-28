/**
 * @file ShaderGenerator.h
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
#include <string>

namespace Jelly {
class JELLY_EXPORT ShaderGenerator : public Object
{
public:
    std::string GetVertexShaderCode() const noexcept;
    std::string GetFragmentShaderCode() const noexcept;

private:
};
} // namespace Jelly
