#include "ShaderGenerator.h"

namespace {
std::string VertexShaderTemplateString = R"(
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
        }
    )";

std::string FragmentShaderTemplateString = R"(
        #version 450

        // FS::In

        // FS::Out

        layout(location = 0) out vec4 FragColor;

        void main()
        {
            // FS::Main Begin
            FragColor = vec4(1., 1., 1., 1.);
            // FS::Main End
        }
    )";
} // namespace

using namespace Jelly;

std::string ShaderGenerator::GetVertexShaderCode() const noexcept
{
    return VertexShaderTemplateString;
}

std::string ShaderGenerator::GetFragmentShaderCode() const noexcept
{
    return FragmentShaderTemplateString;
}
