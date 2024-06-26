#include "DataSetMapper.h"
#include "DataSet.h"
#include "Device.h"
#include "Drawable.h"
#include "Logger.h"
#include "Pipeline.h"
#include "PipelineCache.h"
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

        if (!m_dataSet)
        {
            return;
        }

        PipelineInfo pipelineInfo {
            .strides          = m_dataSet->GetStrides(),
            .depthTestEnable  = false,
            .depthWriteEnable = false,
            .renderPass       = renderPass
        };

        if (m_dataSet->GetPointCount() > 0 && m_dataSet->GetIndexCount() > 0)
        {
            m_drawable = std::make_unique<Drawable>(m_device, m_dataSet);
        }
        else
        {
            Logger::GetInstance()->Warn(std::format(
                "Data set is empty, point count is {}, index count is {}",
                m_dataSet->GetPointCount(),
                m_dataSet->GetIndexCount()
            ));
        }
    }
}

void DataSetMapper::SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept
{
    m_dataSet    = std::move(dataSet);
    m_needUpdate = true;
}
