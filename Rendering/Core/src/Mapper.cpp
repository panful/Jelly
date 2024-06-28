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

void Mapper::SetColorMode(ColorMode colorMode) noexcept
{
    m_colorMode = colorMode;
}

ColorMode Mapper::GetColorMode() const noexcept
{
    return m_colorMode;
}

void Mapper::SetColor(const std::array<double, 3>& color)
{
    m_color = color;
}

std::array<double, 3> Mapper::GetColor() const noexcept
{
    return m_color;
}
