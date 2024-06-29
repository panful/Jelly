#include "SpvCreater.h"
#include "Logger.h"
#include <format>
#include <glslang/SPIRV/GlslangToSpv.h>

namespace {
constexpr TBuiltInResource DefaultTBuiltInResource = {
    .maxLights                                 = 32,
    .maxClipPlanes                             = 6,
    .maxTextureUnits                           = 32,
    .maxTextureCoords                          = 32,
    .maxVertexAttribs                          = 64,
    .maxVertexUniformComponents                = 4096,
    .maxVaryingFloats                          = 64,
    .maxVertexTextureImageUnits                = 32,
    .maxCombinedTextureImageUnits              = 80,
    .maxTextureImageUnits                      = 32,
    .maxFragmentUniformComponents              = 4096,
    .maxDrawBuffers                            = 32,
    .maxVertexUniformVectors                   = 128,
    .maxVaryingVectors                         = 8,
    .maxFragmentUniformVectors                 = 16,
    .maxVertexOutputVectors                    = 16,
    .maxFragmentInputVectors                   = 15,
    .minProgramTexelOffset                     = -8,
    .maxProgramTexelOffset                     = 7,
    .maxClipDistances                          = 8,
    .maxComputeWorkGroupCountX                 = 65535,
    .maxComputeWorkGroupCountY                 = 65535,
    .maxComputeWorkGroupCountZ                 = 65535,
    .maxComputeWorkGroupSizeX                  = 1024,
    .maxComputeWorkGroupSizeY                  = 1024,
    .maxComputeWorkGroupSizeZ                  = 64,
    .maxComputeUniformComponents               = 1024,
    .maxComputeTextureImageUnits               = 16,
    .maxComputeImageUniforms                   = 8,
    .maxComputeAtomicCounters                  = 8,
    .maxComputeAtomicCounterBuffers            = 1,
    .maxVaryingComponents                      = 60,
    .maxVertexOutputComponents                 = 64,
    .maxGeometryInputComponents                = 64,
    .maxGeometryOutputComponents               = 128,
    .maxFragmentInputComponents                = 128,
    .maxImageUnits                             = 8,
    .maxCombinedImageUnitsAndFragmentOutputs   = 8,
    .maxCombinedShaderOutputResources          = 8,
    .maxImageSamples                           = 0,
    .maxVertexImageUniforms                    = 0,
    .maxTessControlImageUniforms               = 0,
    .maxTessEvaluationImageUniforms            = 0,
    .maxGeometryImageUniforms                  = 0,
    .maxFragmentImageUniforms                  = 8,
    .maxCombinedImageUniforms                  = 8,
    .maxGeometryTextureImageUnits              = 16,
    .maxGeometryOutputVertices                 = 256,
    .maxGeometryTotalOutputComponents          = 1024,
    .maxGeometryUniformComponents              = 1024,
    .maxGeometryVaryingComponents              = 64,
    .maxTessControlInputComponents             = 128,
    .maxTessControlOutputComponents            = 128,
    .maxTessControlTextureImageUnits           = 16,
    .maxTessControlUniformComponents           = 1024,
    .maxTessControlTotalOutputComponents       = 4096,
    .maxTessEvaluationInputComponents          = 128,
    .maxTessEvaluationOutputComponents         = 128,
    .maxTessEvaluationTextureImageUnits        = 16,
    .maxTessEvaluationUniformComponents        = 1024,
    .maxTessPatchComponents                    = 120,
    .maxPatchVertices                          = 32,
    .maxTessGenLevel                           = 64,
    .maxViewports                              = 16,
    .maxVertexAtomicCounters                   = 0,
    .maxTessControlAtomicCounters              = 0,
    .maxTessEvaluationAtomicCounters           = 0,
    .maxGeometryAtomicCounters                 = 0,
    .maxFragmentAtomicCounters                 = 8,
    .maxCombinedAtomicCounters                 = 8,
    .maxAtomicCounterBindings                  = 1,
    .maxVertexAtomicCounterBuffers             = 0,
    .maxTessControlAtomicCounterBuffers        = 0,
    .maxTessEvaluationAtomicCounterBuffers     = 0,
    .maxGeometryAtomicCounterBuffers           = 0,
    .maxFragmentAtomicCounterBuffers           = 1,
    .maxCombinedAtomicCounterBuffers           = 1,
    .maxAtomicCounterBufferSize                = 16384,
    .maxTransformFeedbackBuffers               = 4,
    .maxTransformFeedbackInterleavedComponents = 64,
    .maxCullDistances                          = 8,
    .maxCombinedClipAndCullDistances           = 8,
    .maxSamples                                = 4,
    .maxMeshOutputVerticesNV                   = 256,
    .maxMeshOutputPrimitivesNV                 = 512,
    .maxMeshWorkGroupSizeX_NV                  = 32,
    .maxMeshWorkGroupSizeY_NV                  = 1,
    .maxMeshWorkGroupSizeZ_NV                  = 1,
    .maxTaskWorkGroupSizeX_NV                  = 32,
    .maxTaskWorkGroupSizeY_NV                  = 1,
    .maxTaskWorkGroupSizeZ_NV                  = 1,
    .maxMeshViewCountNV                        = 4,
    .maxMeshOutputVerticesEXT                  = 256,
    .maxMeshOutputPrimitivesEXT                = 256,
    .maxMeshWorkGroupSizeX_EXT                 = 128,
    .maxMeshWorkGroupSizeY_EXT                 = 128,
    .maxMeshWorkGroupSizeZ_EXT                 = 128,
    .maxTaskWorkGroupSizeX_EXT                 = 128,
    .maxTaskWorkGroupSizeY_EXT                 = 128,
    .maxTaskWorkGroupSizeZ_EXT                 = 128,
    .maxMeshViewCountEXT                       = 4,
    .maxDualSourceDrawBuffersEXT               = 1,

    .limits =
        {
                 .nonInductiveForLoops                 = 1,
                 .whileLoops                           = 1,
                 .doWhileLoops                         = 1,
                 .generalUniformIndexing               = 1,
                 .generalAttributeMatrixVectorIndexing = 1,
                 .generalVaryingIndexing               = 1,
                 .generalSamplerIndexing               = 1,
                 .generalVariableIndexing              = 1,
                 .generalConstantMatrixVectorIndexing  = 1,
                 }
};
} // namespace

using namespace Jelly;

SpvCreater::SpvCreater() noexcept
{
    auto version = glslang::GetVersion();
    Logger::GetInstance()->Trace(std::format(
        "EsslVersion: {}, GlslVersion: {}, KhronosToolId: {}, Version: {}, {}, {}, {}\n",
        glslang::GetEsslVersionString(),
        glslang::GetGlslVersionString(),
        glslang::GetKhronosToolId(),
        version.major,
        version.minor,
        version.patch,
        version.flavor
    ));

    if (!glslang::InitializeProcess())
    {
        Logger::GetInstance()->Error("failed to init glslang");
    }
}

SpvCreater::~SpvCreater() noexcept
{
    Logger::GetInstance()->Trace("glslang terminate");
    glslang::FinalizeProcess();
}

SpvCreater* SpvCreater::GetInstance() noexcept
{
    static SpvCreater shaderHelper {};
    return &shaderHelper;
}

std::optional<std::vector<uint32_t>>
SpvCreater::GLSL2SPV(const vk::ShaderStageFlagBits shaderType, const std::string_view shaderCode)
{
    EShLanguage stage    = TranslateShaderStage(shaderType);
    EShMessages messages = EShMsgSpvRules;

    const auto shaderStrings = shaderCode.data();

    glslang::TShader shader(stage);
    shader.setStrings(&shaderStrings, 1);

    if (!shader.parse(&DefaultTBuiltInResource, 100, false, messages))
    {
        Logger::GetInstance()->Error(
            std::format("Shader code:\n{}\nShader parse info log:\n{}\n", shaderCode, shader.getInfoLog())
        );
        return {};
    }

    glslang::TProgram program {};
    program.addShader(&shader);
    if (!program.link(messages))
    {
        Logger::GetInstance()->Error(
            std::format("Shader code:\n{}\nProgram link info log:\n{}\n", shaderCode, program.getInfoLog())
        );
        return {};
    }

    std::vector<uint32_t> spv_result {};
    glslang::GlslangToSpv(*program.getIntermediate(stage), spv_result);
    return spv_result;
}

EShLanguage SpvCreater::TranslateShaderStage(vk::ShaderStageFlagBits stage) noexcept
{
    switch (stage)
    {
        case vk::ShaderStageFlagBits::eVertex:
            return EShLangVertex;
        case vk::ShaderStageFlagBits::eTessellationControl:
            return EShLangTessControl;
        case vk::ShaderStageFlagBits::eTessellationEvaluation:
            return EShLangTessEvaluation;
        case vk::ShaderStageFlagBits::eGeometry:
            return EShLangGeometry;
        case vk::ShaderStageFlagBits::eFragment:
            return EShLangFragment;
        case vk::ShaderStageFlagBits::eCompute:
            return EShLangCompute;
        case vk::ShaderStageFlagBits::eRaygenNV:
            return EShLangRayGenNV;
        case vk::ShaderStageFlagBits::eAnyHitNV:
            return EShLangAnyHitNV;
        case vk::ShaderStageFlagBits::eClosestHitNV:
            return EShLangClosestHitNV;
        case vk::ShaderStageFlagBits::eMissNV:
            return EShLangMissNV;
        case vk::ShaderStageFlagBits::eIntersectionNV:
            return EShLangIntersectNV;
        case vk::ShaderStageFlagBits::eCallableNV:
            return EShLangCallableNV;
        case vk::ShaderStageFlagBits::eTaskNV:
            return EShLangTaskNV;
        case vk::ShaderStageFlagBits::eMeshNV:
            return EShLangMeshNV;
        default:
            Logger::GetInstance()->Error("Unknown shader stage");
            return EShLangVertex;
    }
}
