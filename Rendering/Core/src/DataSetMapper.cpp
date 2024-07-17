#include "DataSetMapper.h"
#include "DataArray.h"
#include "DataSet.h"
#include "Device.h"
#include "Drawable.h"
#include "Logger.h"
#include "Pipeline.h"
#include "PipelineCache.h"
#include "SpvCreater.h"
#include "Texture.h"
#include "Viewer.h"
#include <format>

using namespace Jelly;

std::array<double, 6> DataSetMapper::GetBounds() const noexcept
{
    if (m_dataSet)
    {
        return m_dataSet->GetBounds();
    }

    return {-1., 1., -1., 1., -1., 1.};
}

void DataSetMapper::Configure(const std::shared_ptr<Viewer>& viewer) noexcept
{
    Logger::GetInstance()->Debug();

    if (!m_dataSet->HasPointData() || !m_dataSet->HasIndexData())
    {
        return;
    }

    m_shaderCreater->Reset();

    static constexpr uint32_t vertexDimension {3};                                                 // x y z
    static constexpr uint32_t colorComponents {3};                                                 // r g b
    static constexpr uint32_t texCoordComponents {2};                                              // u v

    static constexpr uint32_t sizeOfTransformMats = static_cast<uint32_t>(sizeof(float) * 16 * 3); // Mat4x4 * MVP
    std::vector<PushConstantRange> pushConstantRange {
        {vk::ShaderStageFlagBits::eVertex, sizeOfTransformMats}
    };

    std::vector<uint32_t> strides {vertexDimension * sizeof(float)};
    std::vector<DescriptorSetLayoutBinding> descriptorSetLayoutBindings {};

    uint32_t location {1}; // 0 是 inPos, 其他输入(inColor inNormal...)从1开始
    uint32_t binding {0};  // 描述符集的绑定点
    switch (m_colorMode)
    {
        case ColorMode::Texture:
            if (m_dataSet->HasTexCoordData() && m_texture)
            {
                m_texture->SetDevice(m_device);
                m_texture->Update();
                strides.emplace_back(static_cast<uint32_t>(texCoordComponents * sizeof(float)));

                m_shaderCreater->AddTexture2DColor(location++, binding);
                descriptorSetLayoutBindings.emplace_back(
                    binding++, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment
                );
            }
            else
            {
                Logger::GetInstance()->Error("Color mode is Texture, but not have texture coord data");
            }
            break;
        case ColorMode::Vertex:
            if (m_dataSet->HasColorData())
            {
                strides.emplace_back(static_cast<uint32_t>(colorComponents * sizeof(float)));
                m_shaderCreater->AddVertexColor(location++);
            }
            else
            {
                Logger::GetInstance()->Error("Color mode is Vertex, but not have color data");
            }
            break;
        case ColorMode::Uniform:
            m_shaderCreater->AddUniformColor(binding);
            descriptorSetLayoutBindings.emplace_back(
                binding++, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment
            );
            break;
        default:
            Logger::GetInstance()->Error("Wrong of ColorMode");
            break;
    }

    if (m_enableLighting)
    {
        m_shaderCreater->AddFollowCameraLight(location++);
    }

    auto vertCode = m_shaderCreater->GetVertexShaderCode();
    auto fragCode = m_shaderCreater->GetFragmentShaderCode();

    auto vertSpv = SpvCreater::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eVertex, vertCode);
    auto fragSpv = SpvCreater::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eFragment, fragCode);

    if (!vertSpv)
    {
        Logger::GetInstance()->Error(std::format("failed to compile vertex shader, shader code: {}", vertCode));
        return;
    }
    if (!fragSpv)
    {
        Logger::GetInstance()->Error(std::format("failed to compile fragment shader, shader code: {}", fragCode));
        return;
    }

    PipelineInfo pipelineInfo {
        .vertexShaderCode            = std::move(vertSpv.value()),
        .fragmentShaderCode          = std::move(fragSpv.value()),
        .strides                     = std::move(strides),
        .pushConstantRanges          = std::move(pushConstantRange),
        .descriptorSetLayoutBindings = std::move(descriptorSetLayoutBindings),
        .renderPass                  = viewer->GetRenderPass()
    };

    BuildPipeline(viewer, pipelineInfo);
    m_drawable = std::make_unique<Drawable>(m_device, m_dataSet, m_colorMode);
}

void DataSetMapper::SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept
{
    m_dataSet = std::move(dataSet);
    Changed();
}
