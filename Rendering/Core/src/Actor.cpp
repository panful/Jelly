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

void Actor::SetColor(const std::array<float, 3>& color)
{
    if (m_color != color)
    {
        m_color = color;
        Changed();
    }
}

const std::array<float, 3>& Actor::GetColor() const noexcept
{
    return m_color;
}

void Actor::SetTexture(std::shared_ptr<Texture> texture)
{
    if (m_texture != texture)
    {
        m_texture = std::move(texture);
        Changed();
    }
}

const std::shared_ptr<Texture>& Actor::GetTexture() const noexcept
{
    return m_texture;
}

void Actor::SetEnableLighting(bool enableLighting) noexcept
{
    if (m_enableLighting != enableLighting)
    {
        m_enableLighting = enableLighting;
        Changed();
    }
}

bool Actor::GetEnableLighting() const noexcept
{
    return m_enableLighting;
}

void Actor::SetModelMatrix(const std::array<float, 16>& modelMatrix)
{
    if (m_modelMatrix != modelMatrix)
    {
        m_modelMatrix = modelMatrix;
        Changed();
    }
}

const std::array<float, 16> Actor::GetModelMatrix() const noexcept
{
    return m_modelMatrix;
}
