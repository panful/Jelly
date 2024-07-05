#include "DataSetMapper.h"
#include "DataArray.h"
#include "DataSet.h"
#include "Device.h"
#include "Drawable.h"
#include "Logger.h"
#include "Pipeline.h"
#include "PipelineCache.h"
#include "SpvCreater.h"
#include "Viewer.h"
#include <format>

using namespace Jelly;

void DataSetMapper::Render(const vk::raii::CommandBuffer& commandBuffer, Viewer* viewer) noexcept
{
    Logger::GetInstance()->Debug();

    if (!m_dataSet->HasPointData() || !m_dataSet->HasIndexData())
    {
        return;
    }

    if (m_needUpdate)
    {
        m_needUpdate      = false;
        m_useUniformColor = false;

        static constexpr uint32_t vertexDimension {3};
        static constexpr uint32_t colorComponents {3};

        std::vector<uint32_t> strides {vertexDimension * sizeof(float)};
        if (m_dataSet->HasColorData() && ColorMode::VertexColoring == m_colorMode)
        {
            strides.emplace_back(static_cast<uint32_t>(colorComponents * sizeof(float)));
        }

        std::vector<PushConstantRange> pushConstantRange {};
        uint32_t location {1}; // 0 是 inPos, 其他输入(inColor inNormal...)从1开始
        switch (m_colorMode)
        {
            case ColorMode::ColorMap:
                // TODO
                break;
            case ColorMode::VertexColoring:
                if (m_dataSet->HasColorData())
                {
                    m_shaderCreater->AddPointColor(location);
                    break;
                }
                else
                {
                    Logger::GetInstance()->Warn("Color mode is VertexColoring, but not have color data");
                }
            case ColorMode::UniformColor:
                [[fallthrough]];
            default:
                m_useUniformColor = true;
                m_shaderCreater->AddUniformColor();
                pushConstantRange.emplace_back(
                    vk::ShaderStageFlagBits::eFragment, static_cast<uint32_t>(sizeof(m_color))
                );
                break;
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
            .vertexShaderCode   = std::move(vertSpv.value()),
            .fragmentShaderCode = std::move(fragSpv.value()),
            .strides            = std::move(strides),
            .pushConstantRanges = pushConstantRange,
            .renderPass         = viewer->GetRenderPass()
        };

        BuildPipeline(pipelineInfo);

        m_drawable = std::make_unique<Drawable>(m_device, m_dataSet);
    }

    auto&& pipeline = m_device->GetPipelineCache()->GetPipeline(m_pipelineKey);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());
    if (m_useUniformColor)
    {
        commandBuffer.pushConstants<float>(
            pipeline->GetPipelineLayout(), vk::ShaderStageFlagBits::eFragment, 0u, m_color
        );
    }
    commandBuffer.bindVertexBuffers(0, m_drawable->GetVertexBuffers(), m_drawable->GetVertexOffsets());
    commandBuffer.bindIndexBuffer(m_drawable->GetIndexBuffer(), 0, m_drawable->GetIndexType());
    commandBuffer.drawIndexed(m_drawable->GetIndexCount(), 1, 0, 0, 0);
}

void DataSetMapper::SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept
{
    m_dataSet    = std::move(dataSet);
    m_needUpdate = true;
}
