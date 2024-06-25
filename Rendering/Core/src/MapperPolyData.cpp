#include "MapperPolyData.h"
#include "Device.h"
#include "Logger.h"
#include "Pipeline.h"
#include "PipelineCache.h"

using namespace Jelly;

void MapperPolyData::Render(
    const vk::raii::CommandBuffer& commandBuffer, const vk::raii::RenderPass& renderPass
) noexcept
{
    Logger::GetInstance()->Debug();

    if (m_needUpdate)
    {
        m_needUpdate = false;
    }
}
