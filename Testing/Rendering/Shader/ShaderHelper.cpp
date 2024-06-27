#include "ShaderHelper.h"
#include <gtest/gtest.h>
#include <string_view>

TEST(Test_ShaderHelper, vertexShader)
{
    std::string_view glslCode =
        R"(
            #version 450
            layout(location = 0) in vec2 inPos;
            layout(location = 1) in vec3 inColor;

            layout(location = 0) out vec3 fragColor;

            void main() 
            {
                gl_Position = vec4(inPos, 0.0, 1.0);
                fragColor = inColor;
            }
        )";

    std::optional<std::vector<uint32_t>> spv =
        Jelly::ShaderHelper::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eVertex, glslCode);

    EXPECT_TRUE(spv.has_value());
}

TEST(Test_ShaderHelper, fragmenShader)
{
    std::string_view glslCode =
        R"(
            #version 450

            layout(location = 0) in vec3 fragColor;

            layout(location = 0) out vec4 outColor;

            void main() {
                outColor = vec4(fragColor, 1.0);
            }
        )";

    std::optional<std::vector<uint32_t>> spv =
        Jelly::ShaderHelper::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eFragment, glslCode);

    EXPECT_TRUE(spv.has_value());
}
