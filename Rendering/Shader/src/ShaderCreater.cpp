#include "ShaderCreater.h"
#include "Logger.h"
#include <format>

namespace {
std::string VertexShaderTemplateString = R"(// Vertex Shader
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

std::string FragmentShaderTemplateString = R"(// Fragment Shader
#version 450

// FS::In

// FS::Out
layout(location = 0) out vec4 FragColor;

void main()
{
    // FS::Main Begin
    FragColor = vec4(1., 1., 1., 1.);
    // FS::Main End
})";
} // namespace

using namespace Jelly;

ShaderCreater::ShaderCreater()
    : m_vertexShaderCode(VertexShaderTemplateString)
    , m_fragmentShaderCode(FragmentShaderTemplateString)
{
}

void ShaderCreater::Reset()
{
    m_vertexShaderCode   = VertexShaderTemplateString;
    m_fragmentShaderCode = FragmentShaderTemplateString;
}

const std::string& ShaderCreater::GetVertexShaderCode() const noexcept
{
    return m_vertexShaderCode;
}

const std::string& ShaderCreater::GetFragmentShaderCode() const noexcept
{
    return m_fragmentShaderCode;
}

void ShaderCreater::AddVertexColor(uint32_t location)
{
    ReplaceValue(
        m_vertexShaderCode,
        "// Layout::Color",
        std::format(
            "layout(location = {}) in vec3 inColor;\nlayout(location = {}) out vec3 vsOutColor;", location, location
        )
    );

    ReplaceValue(m_vertexShaderCode, "// VS::Main Begin", "// VS::Main Begin\n    vsOutColor = inColor;");

    ReplaceValue(
        m_fragmentShaderCode, "// FS::In", std::format("// FS::In\nlayout(location = {}) in vec3 fsInColor;", location)
    );
    ReplaceValue(m_fragmentShaderCode, "FragColor = ", ";", "vec4(fsInColor, 1.)");
}

void ShaderCreater::AddUniformColor(uint32_t location)
{
    ReplaceValue(
        m_fragmentShaderCode,
        "// FS::In",
        std::format("// FS::In\nlayout(binding = {}) uniform UBO {{\n    vec3 color;\n}} uColor;", location)
    );
    ReplaceValue(m_fragmentShaderCode, "FragColor = ", ";", "vec4(uColor.color, 1.)");
}

void ShaderCreater::AddTexture2DColor(uint32_t location, uint32_t binding)
{
    ReplaceValue(
        m_vertexShaderCode,
        "// Layout::TexCoord",
        std::format(
            "layout(location = {}) in vec2 inTexCoord;\nlayout(location = {}) out vec2 vsOutTexCoord;",
            location,
            location
        )
    );

    ReplaceValue(m_vertexShaderCode, "// VS::Main Begin", "// VS::Main Begin\n    vsOutTexCoord = inTexCoord;");

    ReplaceValue(
        m_fragmentShaderCode,
        "// FS::In",
        std::format(
            "// FS::In\nlayout(location = {}) in vec2 fsInTexCoord;\nlayout(binding = {}) uniform sampler2D "
            "uTexSampler2D;",
            location,
            binding
        )
    );

    ReplaceValue(m_fragmentShaderCode, "FragColor = ", ";", "texture(uTexSampler2D, fsInTexCoord)");
}

void ShaderCreater::AddFollowCameraLight(uint32_t location)
{
    ReplaceValue(
        m_vertexShaderCode,
        "// VS::Out",
        std::format("// VS::Out\nlayout(location = {}) out vec3 vsOutViewPos;", location)
    );

    ReplaceValue(
        m_vertexShaderCode,
        "// VS::Main Begin",
        "// VS::Main Begin\n    vsOutViewPos = vec3(pcMVP.view * pcMVP.model * vec4(inPos, 1.));"
    );

    ReplaceValue(
        m_fragmentShaderCode,
        "// FS::In",
        std::format("// FS::In\nlayout(location = {}) in vec3 fsInViewPos;", location)
    );

    ReplaceValue(
        m_fragmentShaderCode,
        "// FS::Main End",
        "vec3 dx = dFdx(fsInViewPos);\n    vec3 dy = dFdy(fsInViewPos);\n    vec3 normal = "
        "normalize(cross(dx, dy));\n    vec3 lightColor = vec3(1.);\n    lightColor *= max(0., -normal.z);\n\n    "
        "FragColor = vec4(FragColor.xyz * lightColor, 1.);\n    // FS::Main End"
    );
}

void ShaderCreater::ReplaceValue(
    std::string& str, const std::string& start, const std::string& end, const std::string& replacement
)
{
    if (size_t startPos = str.find(start); startPos != std::string::npos)
    {
        if (size_t endPos = str.find(end, startPos + 1); endPos != std::string::npos)
        {
            str.replace(startPos + start.length(), endPos - startPos - start.length(), replacement);
            return;
        }
    }

    Logger::GetInstance()->Error(std::format("{} or {} not found in {}", start, end, str));
}

void ShaderCreater::ReplaceValue(std::string& str, const std::string& oldStr, const std::string& newStr)
{
    if (auto pos = str.find(oldStr); pos != std::string::npos)
    {
        str.replace(pos, oldStr.length(), newStr);
    }
    else
    {
        Logger::GetInstance()->Error(std::format("{} not found in {}", oldStr, str));
    }
}
