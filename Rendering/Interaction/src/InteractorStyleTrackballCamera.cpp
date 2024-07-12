#include "InteractorStyleTrackballCamera.h"
#include "Interactor.h"
#include "Renderer.h"
#include "Window.h"

using namespace Jelly;

void InteractorStyleTrackballCamera::MouseMoveEvent()
{
    if (auto renderer = m_currentRenderer.lock())
    {
        auto interactor = m_interactor.lock();
        auto camera     = renderer->GetCamera();

        switch (m_state)
        {
            case InteractorState::Rotate:
            {
                auto dx = interactor->GetMousePosition()[0] - m_lastMousePosition[0];
                auto dy = interactor->GetMousePosition()[1] - m_lastMousePosition[1];

                static constexpr auto ratio = std::numbers::pi_v<double> / -180.; // 相机是逆时针旋转，因此此处取负值

                camera->Azimuth(dx * ratio);
                camera->Elevation(dy * ratio);
                renderer->ResetCameraClipRange();
                interactor->GetWindow()->Render();
            }
            break;
            case InteractorState::Pan:
            {
                auto newPoint  = renderer->DisplayToWorld(interactor->GetMousePosition());
                auto oldPoint  = renderer->DisplayToWorld(m_lastMousePosition);
                auto& eyePos   = camera->GetEyePos();
                auto& focalPos = camera->GetFocalPos();

                std::array<double, 3> motionVector {
                    oldPoint[0] - newPoint[0], oldPoint[1] - newPoint[1], oldPoint[2] - newPoint[2]
                };

                camera->SetEyePos(
                    {eyePos[0] + motionVector[0], eyePos[1] + motionVector[1], eyePos[2] + motionVector[2]}
                );
                camera->SetFocalPos(
                    {focalPos[0] + motionVector[0], focalPos[1] + motionVector[1], focalPos[2] + motionVector[2]}
                );
                interactor->GetWindow()->Render();
            }
            break;
            case InteractorState::Zoom:
            {
            }
            break;
            default:
                break;
        }

        m_lastMousePosition = interactor->GetMousePosition();
    }
}

void InteractorStyleTrackballCamera::RightButtonPressEvent()
{
    FindPokedRenderer();

    if (auto interactor = m_interactor.lock())
    {
        m_lastMousePosition = interactor->GetMousePosition();
        m_state             = InteractorState::Rotate;
    }
}

void InteractorStyleTrackballCamera::RightButtonReleaseEvent()
{
    m_state = InteractorState::None;
}

void InteractorStyleTrackballCamera::LeftButtonPressEvent()
{
    FindPokedRenderer();

    if (auto interactor = m_interactor.lock())
    {
        m_lastMousePosition = interactor->GetMousePosition();
        m_state             = InteractorState::Pan;
    }
}

void InteractorStyleTrackballCamera::LeftButtonReleaseEvent()
{
    m_state = InteractorState::None;
}

void InteractorStyleTrackballCamera::MiddleButtonPressEvent()
{
}

void InteractorStyleTrackballCamera::MiddleButtonReleaseEvent()
{
}

void InteractorStyleTrackballCamera::MouseWheelForwardEvent()
{
    FindPokedRenderer();

    if (auto renderer = m_currentRenderer.lock())
    {
        auto interactor = m_interactor.lock();
        Dolly(1. + m_zoomFactor);
        interactor->GetWindow()->Render();
    }
}

void InteractorStyleTrackballCamera::MouseWheelBackwardEvent()
{
    FindPokedRenderer();

    if (auto renderer = m_currentRenderer.lock())
    {
        auto interactor = m_interactor.lock();
        Dolly(1. - m_zoomFactor);
        interactor->GetWindow()->Render();
    }
}
