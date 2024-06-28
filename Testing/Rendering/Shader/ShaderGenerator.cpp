#include "ShaderGenerator.h"
#include <gtest/gtest.h>
#include <string>

using namespace std::string_literals;

TEST(Test_ShaderGenerator, addPointColor)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

layout(location = 1) in vec3 inColor;
layout(location = 1) out vec3 vsOutColor;

// Layout::Normal

// Layout::TexCoord

void main()
{
    // VS::Main Begin
    vsOutColor = inColor;
    gl_Position = vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(location = 1) in vec3 fsInColor;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(fsInColor, 1.);
    // FS::Main End
})";

    Jelly::ShaderGenerator generator {};
    generator.AddPointColor(1);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}
