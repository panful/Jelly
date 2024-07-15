#include "Object.h"

using namespace Jelly;

void Object::Changed() noexcept
{
    m_isChanged = true;
}

bool Object::IsChanged() const noexcept
{
    return m_isChanged;
}

void Object::ResetChanged() noexcept
{
    m_isChanged = false;
}
