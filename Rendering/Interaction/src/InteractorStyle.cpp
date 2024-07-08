#include "InteractorStyle.h"
#include "Interactor.h"
#include "Renderer.h"
#include "Window.h"

using namespace Jelly;

void InteractorStyle::SetInteractor(std::shared_ptr<Interactor> interactor) noexcept
{
    m_interactor = std::move(interactor);
}

void InteractorStyle::FindPokedRenderer()
{
    if (auto interactor = m_interactor.lock())
    {
        for (const auto& renderer : interactor->GetWindow()->GetAllRenderers())
        {
            if (renderer->IsInViewport(interactor->GetMousePosition()))
            {
                m_currentRenderer = renderer;
                return;
            }
        }
    }

    m_currentRenderer.reset();
}

void InteractorStyle::MouseMoveEvent()
{
}

void InteractorStyle::RightButtonPressEvent()
{
}

void InteractorStyle::RightButtonReleaseEvent()
{
}

void InteractorStyle::LeftButtonPressEvent()
{
}

void InteractorStyle::LeftButtonReleaseEvent()
{
}

void InteractorStyle::MiddleButtonPressEvent()
{
}

void InteractorStyle::MiddleButtonReleaseEvent()
{
}

void InteractorStyle::MouseWheelForwardEvent()
{
}

void InteractorStyle::MouseWheelBackwardEvent()
{
}

void InteractorStyle::CharEvent()
{
}

void InteractorStyle::KeyPressEvent()
{
}

void InteractorStyle::KeyReleaseEvent()
{
}

void InteractorStyle::EnterEvent()
{
}

void InteractorStyle::LeaveEvent()
{
}
