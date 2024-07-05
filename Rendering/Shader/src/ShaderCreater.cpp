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

void main()
{
    // VS::Main Begin
    gl_Position = vec4(inPos, 1.);
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

std::string ShaderCreater::GetVertexShaderCode() const noexcept
{
    return m_vertexShaderCode;
}

std::string ShaderCreater::GetFragmentShaderCode() const noexcept
{
    return m_fragmentShaderCode;
}

void ShaderCreater::AddPointColor(uint32_t location)
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

void ShaderCreater::RemovePointColor()
{
}

void ShaderCreater::SetFragColor(const std::array<double, 3>& color)
{
    ReplaceValue(
        m_fragmentShaderCode, "FragColor = ", ";", std::format("vec4({}, {}, {}, 1.)", color[0], color[1], color[2])
    );
}

void ShaderCreater::AddUniformColor()
{
    ReplaceValue(
        m_fragmentShaderCode,
        "// FS::In",
        "// FS::In\nlayout(push_constant) uniform PushConstant {\n    vec3 color;\n} pcColor;"
    );
    ReplaceValue(m_fragmentShaderCode, "FragColor = ", ";", "vec4(pcColor.color, 1.)");
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
