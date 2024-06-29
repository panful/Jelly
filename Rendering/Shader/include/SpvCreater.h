/**
 * @file SpvCreater.h
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
#include <glslang/Public/ShaderLang.h>
#include <optional>
#include <string_view>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {

class JELLY_EXPORT SpvCreater : public Object
{
private:
    SpvCreater() noexcept;
    ~SpvCreater() noexcept override;

public:
    static SpvCreater* GetInstance() noexcept;

    std::optional<std::vector<uint32_t>>
    GLSL2SPV(const vk::ShaderStageFlagBits shaderType, const std::string_view shaderCode);

private:
    static EShLanguage TranslateShaderStage(vk::ShaderStageFlagBits stage) noexcept;
};
} // namespace Jelly
