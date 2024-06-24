#include "Actor.h"

using namespace Jelly;

void Actor::SetDevice(std::shared_ptr<Device> device) noexcept
{
    m_device = std::move(device);
}

void Actor::SetMapper(std::shared_ptr<Mapper> mapper) noexcept
{
    m_mapper = std::move(mapper);
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