#include "Mapper.h"

using namespace Jelly;

void Mapper::SetDevice(std::shared_ptr<Device> device) noexcept
{
    m_device = std::move(device);
}
