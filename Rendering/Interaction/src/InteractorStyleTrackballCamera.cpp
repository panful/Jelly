#include "InteractorStyleTrackballCamera.h"
#include "Interactor.h"
#include "Renderer.h"
#include "Window.h"

using namespace Jelly;

void InteractorStyleTrackballCamera::MouseMoveEvent()
{
    if (auto interactor = m_interactor.lock())
    {
        switch (m_state)
        {
            case InteractorState::Rotate:
            {
                break;
            }
            case InteractorState::Pan:
            {
                break;
            }
            case InteractorState::Zoom:
            {
                break;
            }
            default:
                break;
        }
    }
}

void InteractorStyleTrackballCamera::RightButtonPressEvent()
{
    FindPokedRenderer();

    if (auto renderer = m_currentRenderer.lock())
    {
        auto camera = renderer->GetCamera();
    }
}

void InteractorStyleTrackballCamera::RightButtonReleaseEvent()
{
}

void InteractorStyleTrackballCamera::LeftButtonPressEvent()
{
}

void InteractorStyleTrackballCamera::LeftButtonReleaseEvent()
{
}

void InteractorStyleTrackballCamera::MiddleButtonPressEvent()
{
}

void InteractorStyleTrackballCamera::MiddleButtonReleaseEvent()
{
}

void InteractorStyleTrackballCamera::MouseWheelForwardEvent()
{
}

void InteractorStyleTrackballCamera::MouseWheelBackwardEvent()
{
}
