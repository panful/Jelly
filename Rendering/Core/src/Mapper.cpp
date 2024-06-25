#include "Mapper.h"
#include "Device.h"
#include "Pipeline.h"
#include <functional>

using namespace Jelly;

void Mapper::SetDevice(std::shared_ptr<Device> device) noexcept
{
    m_device = std::move(device);
}

void Mapper::BuildPipeline(const vk::raii::RenderPass& renderPass) noexcept
{
    PipelineInfo pipelineInfo {.renderPass = renderPass};

    m_device->GetPipelineCache()->AddPipeline(
        std::hash<PipelineInfo>()(pipelineInfo), std::make_shared<Pipeline>(m_device, pipelineInfo)
    );
}
