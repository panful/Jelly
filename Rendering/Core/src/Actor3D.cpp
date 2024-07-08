#include "Actor3D.h"
#include "Logger.h"
#include "Mapper.h"

using namespace Jelly;

void Actor3D::Render(
    const vk::raii::CommandBuffer& commandBuffer, const std::shared_ptr<Viewer>& viewer, Renderer* renderer
) noexcept
{
    Logger::GetInstance()->Debug();

    m_mapper->Render(commandBuffer, viewer, renderer);
}
