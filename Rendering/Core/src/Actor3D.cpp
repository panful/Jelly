#include "Actor3D.h"
#include "Logger.h"
#include "Mapper.h"

using namespace Jelly;

void Actor3D::Render(const vk::raii::CommandBuffer& commandBuffer, const vk::raii::RenderPass& renderPass) noexcept
{
    Logger::GetInstance()->Debug();

    m_mapper->Render(commandBuffer, renderPass);
}
