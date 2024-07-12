#include "Interactor.h"
#include "InteractorStyle.h"

using namespace Jelly;

void Interactor::SetWindow(std::shared_ptr<Window> window) noexcept
{
    m_window = std::move(window);
}

std::shared_ptr<Window> Interactor::GetWindow() const noexcept
{
    return m_window;
}

void Interactor::SetInteractorStyle(std::shared_ptr<InteractorStyle> interactorStyle) noexcept
{
    m_interactorStyle = std::move(interactorStyle);
    m_interactorStyle->SetInteractor(weak_from_this());
}

const std::array<int, 2>& Interactor::GetMousePosition() const noexcept
{
    return m_mousePosition;
}
