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

const std::shared_ptr<Mapper>& Actor::GetMapper() const noexcept
{
    return m_mapper;
}

void Actor::SetVisibility(bool visible) noexcept
{
    if (m_visibility != visible)
    {
        m_visibility = visible;
        Changed();
    }
}

bool Actor::GetVisibility() const noexcept
{
    return m_visibility;
}

std::array<double, 6> Actor::GetBounds() const noexcept
{
    if (m_mapper)
    {
        return m_mapper->GetBounds();
    }

    return {-1., 1., -1., 1., -1., 1.};
}
