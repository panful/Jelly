#include "ShaderCreater.h"
#include <gtest/gtest.h>
#include <string>

TEST(Test_ShaderGenerator, addVertexColor)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

layout(location = 1) in vec3 inColor;
layout(location = 1) out vec3 vsOutColor;

// Layout::Normal

// Layout::TexCoord

// VS::Out

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    vsOutColor = inColor;
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
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
    generator.AddVertexColor(1);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, addUniformColor)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

// Layout::Color

// Layout::Normal

// Layout::TexCoord

// VS::Out

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(binding = 9) uniform UBO {
    vec3 color;
} uColor;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(uColor.color, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.AddUniformColor(9);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, addFollowCameraLight)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

// Layout::Color

// Layout::Normal

// Layout::TexCoord

// VS::Out
layout(location = 5) out vec3 vsOutViewPos;

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    vsOutViewPos = vec3(pcMVP.view * pcMVP.model * vec4(inPos, 1.));
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(location = 5) in vec3 fsInViewPos;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(1., 1., 1., 1.);
    vec3 dx = dFdx(fsInViewPos);
    vec3 dy = dFdy(fsInViewPos);
    vec3 normal = normalize(cross(dx, dy));
    vec3 lightColor = vec3(1.);
    lightColor *= max(0., -normal.z);

    FragColor = vec4(FragColor.xyz * lightColor, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.AddFollowCameraLight(5);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, addFollowCameraLight_addVertexColor)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

layout(location = 1) in vec3 inColor;
layout(location = 1) out vec3 vsOutColor;

// Layout::Normal

// Layout::TexCoord

// VS::Out
layout(location = 3) out vec3 vsOutViewPos;

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    vsOutColor = inColor;
    vsOutViewPos = vec3(pcMVP.view * pcMVP.model * vec4(inPos, 1.));
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(location = 1) in vec3 fsInColor;
layout(location = 3) in vec3 fsInViewPos;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(fsInColor, 1.);
    vec3 dx = dFdx(fsInViewPos);
    vec3 dy = dFdy(fsInViewPos);
    vec3 normal = normalize(cross(dx, dy));
    vec3 lightColor = vec3(1.);
    lightColor *= max(0., -normal.z);

    FragColor = vec4(FragColor.xyz * lightColor, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.AddFollowCameraLight(3);
    generator.AddVertexColor(1);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, addVertexColor_addFollowCameraLight)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

layout(location = 1) in vec3 inColor;
layout(location = 1) out vec3 vsOutColor;

// Layout::Normal

// Layout::TexCoord

// VS::Out
layout(location = 3) out vec3 vsOutViewPos;

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    vsOutViewPos = vec3(pcMVP.view * pcMVP.model * vec4(inPos, 1.));
    vsOutColor = inColor;
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(location = 3) in vec3 fsInViewPos;
layout(location = 1) in vec3 fsInColor;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(fsInColor, 1.);
    vec3 dx = dFdx(fsInViewPos);
    vec3 dy = dFdy(fsInViewPos);
    vec3 normal = normalize(cross(dx, dy));
    vec3 lightColor = vec3(1.);
    lightColor *= max(0., -normal.z);

    FragColor = vec4(FragColor.xyz * lightColor, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.AddVertexColor(1);
    generator.AddFollowCameraLight(3);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, addFollowCameraLight_addUniformColor)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

// Layout::Color

// Layout::Normal

// Layout::TexCoord

// VS::Out
layout(location = 5) out vec3 vsOutViewPos;

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    vsOutViewPos = vec3(pcMVP.view * pcMVP.model * vec4(inPos, 1.));
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(location = 5) in vec3 fsInViewPos;
layout(binding = 9) uniform UBO {
    vec3 color;
} uColor;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(uColor.color, 1.);
    vec3 dx = dFdx(fsInViewPos);
    vec3 dy = dFdy(fsInViewPos);
    vec3 normal = normalize(cross(dx, dy));
    vec3 lightColor = vec3(1.);
    lightColor *= max(0., -normal.z);

    FragColor = vec4(FragColor.xyz * lightColor, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.AddUniformColor(9);
    generator.AddFollowCameraLight(5);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, addUniformColor_addFollowCameraLight)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

// Layout::Color

// Layout::Normal

// Layout::TexCoord

// VS::Out
layout(location = 5) out vec3 vsOutViewPos;

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    vsOutViewPos = vec3(pcMVP.view * pcMVP.model * vec4(inPos, 1.));
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(binding = 9) uniform UBO {
    vec3 color;
} uColor;
layout(location = 5) in vec3 fsInViewPos;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(uColor.color, 1.);
    vec3 dx = dFdx(fsInViewPos);
    vec3 dy = dFdy(fsInViewPos);
    vec3 normal = normalize(cross(dx, dy));
    vec3 lightColor = vec3(1.);
    lightColor *= max(0., -normal.z);

    FragColor = vec4(FragColor.xyz * lightColor, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.AddFollowCameraLight(5);
    generator.AddUniformColor(9);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, addTexture2DColor)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

// Layout::Color

// Layout::Normal

layout(location = 7) in vec2 inTexCoord;
layout(location = 7) out vec2 vsOutTexCoord;

// VS::Out

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    vsOutTexCoord = inTexCoord;
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(location = 7) in vec2 fsInTexCoord;
layout(binding = 3) uniform sampler2D uTexSampler2D;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = texture(uTexSampler2D, fsInTexCoord);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.AddTexture2DColor(7, 3);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}

TEST(Test_ShaderGenerator, addTexture2DColor_addFollowCameraLight)
{
    std::string vertCode = R"(// Vertex Shader
#version 450

layout(location = 0) in vec3 inPos;

// Layout::Color

// Layout::Normal

layout(location = 2) in vec2 inTexCoord;
layout(location = 2) out vec2 vsOutTexCoord;

// VS::Out
layout(location = 5) out vec3 vsOutViewPos;

layout(push_constant) uniform PushConstant {
    mat4 model;
    mat4 view;
    mat4 proj;
} pcMVP;

void main()
{
    // VS::Main Begin
    vsOutViewPos = vec3(pcMVP.view * pcMVP.model * vec4(inPos, 1.));
    vsOutTexCoord = inTexCoord;
    gl_Position = pcMVP.proj * pcMVP.view * pcMVP.model * vec4(inPos, 1.);
    // VS::Main End
})";

    std::string fragCode = R"(// Fragment Shader
#version 450

// FS::In
layout(location = 5) in vec3 fsInViewPos;
layout(location = 2) in vec2 fsInTexCoord;
layout(binding = 3) uniform sampler2D uTexSampler2D;

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = texture(uTexSampler2D, fsInTexCoord);
    vec3 dx = dFdx(fsInViewPos);
    vec3 dy = dFdy(fsInViewPos);
    vec3 normal = normalize(cross(dx, dy));
    vec3 lightColor = vec3(1.);
    lightColor *= max(0., -normal.z);

    FragColor = vec4(FragColor.xyz * lightColor, 1.);
    // FS::Main End
})";

    Jelly::ShaderCreater generator {};
    generator.AddTexture2DColor(2, 3);
    generator.AddFollowCameraLight(5);

    EXPECT_EQ(vertCode, generator.GetVertexShaderCode());
    EXPECT_EQ(fragCode, generator.GetFragmentShaderCode());
}
