#include "Actor3D.h"
#include "Mapper.h"
#include "Logger.h"

using namespace Jelly;

void Actor3D::Render() noexcept
{
    Logger::GetInstance()->Debug();

    m_mapper->Render();
}
