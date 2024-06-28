#include "DataSetMapper.h"
#include "DataSet.h"
#include "Device.h"
#include "Drawable.h"
#include "Logger.h"
#include "Pipeline.h"
#include "PipelineCache.h"
#include "ShaderHelper.h"
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

        auto vertCode = m_shaderGenerator->GetVertexShaderCode();
        auto fragCode = m_shaderGenerator->GetFragmentShaderCode();

        auto vertSpv = ShaderHelper::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eVertex, vertCode);
        auto fragSpv = ShaderHelper::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eFragment, fragCode);

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
    commandBuffer.bindVertexBuffers(0, m_drawable->GetVertexBuffers(), {0});
    commandBuffer.bindIndexBuffer(m_drawable->GetIndexBuffer(), 0, m_drawable->GetIndexType());
    commandBuffer.drawIndexed(m_drawable->GetIndexCount(), 1, 0, 0, 0);
}

void DataSetMapper::SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept
{
    m_dataSet    = std::move(dataSet);
    m_needUpdate = true;
}
