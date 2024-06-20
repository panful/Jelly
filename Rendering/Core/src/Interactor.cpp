#include "Interactor.h"

using namespace Jelly;

void Interactor::SetWindow(std::shared_ptr<Window> window) noexcept
{
    m_window = std::move(window);
}
