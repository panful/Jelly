#include "Mapper.h"
#include "Device.h"
#include <functional>

using namespace Jelly;

void Mapper::SetDevice(std::shared_ptr<Device> device) noexcept
{
    m_device = std::move(device);
}

void Mapper::BuildPipeline(const vk::raii::RenderPass& renderPass, const PipelineInfo& pipelineInfo) noexcept
{
    m_pipelineKey = std::hash<PipelineInfo>()(pipelineInfo);

    if (!m_device->GetPipelineCache()->HasPipeline(m_pipelineKey))
    {
        m_device->GetPipelineCache()->AddPipeline(m_pipelineKey, std::make_shared<Pipeline>(m_device, pipelineInfo));
    }
}
