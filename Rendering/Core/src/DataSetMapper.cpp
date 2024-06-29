#include "DataSetMapper.h"
#include "DataSet.h"
#include "Device.h"
#include "Drawable.h"
#include "Logger.h"
#include "Pipeline.h"
#include "PipelineCache.h"
#include "SpvCreater.h"
#include <format>

using namespace Jelly;

void DataSetMapper::Render(
    const vk::raii::CommandBuffer& commandBuffer, const vk::raii::RenderPass& renderPass
) noexcept
{
    Logger::GetInstance()->Debug();

    if (m_needUpdate)
    {
        m_needUpdate = false;

        uint32_t location {1}; // 0 是 inPos, 其他输入从1开始

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
                m_shaderCreater->SetFragColor(m_color);
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
            .vertexShaderCode   = vertSpv.value(),
            .fragmentShaderCode = fragSpv.value(),
            .strides            = m_dataSet->GetStrides(),
            .depthTestEnable    = false,
            .depthWriteEnable   = false,
            .renderPass         = renderPass
        };

        BuildPipeline(renderPass, pipelineInfo);

        m_drawable = std::make_unique<Drawable>(m_device, m_dataSet);
    }

    auto pipeline = m_device->GetPipelineCache()->GetPipeline(m_pipelineKey);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());
    commandBuffer.bindVertexBuffers(0, m_drawable->GetVertexBuffers(), m_drawable->GetVertexOffsets());
    commandBuffer.bindIndexBuffer(m_drawable->GetIndexBuffer(), 0, m_drawable->GetIndexType());
    commandBuffer.drawIndexed(m_drawable->GetIndexCount(), 1, 0, 0, 0);
}

void DataSetMapper::SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept
{
    m_dataSet    = std::move(dataSet);
    m_needUpdate = true;
}
