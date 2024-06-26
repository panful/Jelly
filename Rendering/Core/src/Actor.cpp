#include "Actor.h"
#include "Mapper.h"

using namespace Jelly;

void Actor::SetDevice(std::shared_ptr<Device> device) noexcept
{
    m_device = std::move(device);
    m_mapper->SetDevice(m_device);
}

void Actor::SetMapper(std::shared_ptr<Mapper> mapper) noexcept
{
    m_mapper = std::move(mapper);
    m_mapper->SetDevice(m_device);
}

void Actor::SetVisibility(bool visible) noexcept
{
    if (m_visibility != visible)
    {
        m_visibility = visible;
        m_needUpdate = true;
    }
}

bool Actor::GetVisibility() const noexcept
{
    return m_visibility;
}
