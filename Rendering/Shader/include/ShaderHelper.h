/**
 * @file ShaderHelper.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>
#include <vulkan/vulkan_raii.hpp>
#include <glslang/Public/ShaderLang.h>

namespace Jelly {

class JELLY_EXPORT ShaderHelper : public Object
{
private:
    ShaderHelper() noexcept;
    ~ShaderHelper() noexcept override;

public:
    static ShaderHelper* GetInstance() noexcept;

    std::optional<std::vector<uint32_t>>
    GLSL2SPV(const vk::ShaderStageFlagBits shaderType, const std::string_view shaderCode);

private:
    static EShLanguage TranslateShaderStage(vk::ShaderStageFlagBits stage) noexcept;
};
} // namespace Jelly
