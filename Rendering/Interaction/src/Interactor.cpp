#include "Interactor.h"
#include "InteractorStyle.h"

using namespace Jelly;

void Interactor::SetWindow(std::shared_ptr<Window> window) noexcept
{
    m_window = std::move(window);
}

void Interactor::SetInteractorStyle(std::shared_ptr<InteractorStyle> interactorStyle) noexcept
{
    m_interactorStyle = std::move(interactorStyle);
    m_interactorStyle->SetInteractor(shared_from_this());
}
