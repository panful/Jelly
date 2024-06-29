#include "ShaderCreater.h"
#include <gtest/gtest.h>
#include <string>

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

    Jelly::ShaderCreater generator {};
    generator.AddPointColor(1);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, setFragColor)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

// Layout::Color

// Layout::Normal

// Layout::TexCoord

void main()
{
    // VS::Main Begin
    gl_Position = vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(0.1, 0.2, 0.3, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.SetFragColor({.1, .2, .3});

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, setFragColor2)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

// Layout::Color

// Layout::Normal

// Layout::TexCoord

void main()
{
    // VS::Main Begin
    gl_Position = vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(1.1, 9.99, 0.001, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.SetFragColor({1.1, 9.99, .001});

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}
