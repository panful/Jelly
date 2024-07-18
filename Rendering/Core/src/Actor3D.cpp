#include "Actor3D.h"
#include "Logger.h"
#include "Mapper.h"

using namespace Jelly;

void Actor3D::Render(const vk::raii::CommandBuffer& commandBuffer, Renderer* renderer) noexcept
{
    Logger::GetInstance()->Debug();

    if (IsChanged())
    {
        m_mapper->Changed();
        ResetChanged();
    }

    m_mapper->Render(commandBuffer, renderer, this);
}
