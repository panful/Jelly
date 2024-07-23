#include "InteractorStyle.h"
#include "Camera.h"
#include "Interactor.h"
#include "Logger.h"
#include "Renderer.h"
#include "Window.h"
#include <cmath>

using namespace Jelly;

void InteractorStyle::SetInteractor(std::weak_ptr<Interactor> interactor) noexcept
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

void InteractorStyle::Dolly(const std::shared_ptr<Renderer>& renderer, double factor) const noexcept
{
    if (factor <= 0.0)
    {
        return;
    }

    auto camera = renderer->GetCamera();
    switch (camera->GetCameraType())
    {
        case CameraType::Orthographic:
        {
            camera->SetOrthographicScale(camera->GetOrthographicScale() / factor);
        }
        break;
        case CameraType::Perspective:
        {
            const auto& eyePos   = camera->GetEyePos();
            const auto& focalPos = camera->GetFocalPos();

            auto x = focalPos[0] - eyePos[0];
            auto y = focalPos[1] - eyePos[1];
            auto z = focalPos[2] - eyePos[2];

            std::array<double, 3> newEyePos = {
                focalPos[0] - x / factor, focalPos[1] - y / factor, focalPos[2] - z / factor
            };

            camera->SetEyePos(newEyePos);
            renderer->ResetCameraClipRange();
        }
        break;
        default:
        {
            Logger::GetInstance()->Warn("wrong camera type");
        }
        break;
    }
}

void InteractorStyle::TranslateCamera(
    const std::shared_ptr<Renderer>& renderer, const std::array<double, 3>& from, const std::array<double, 3>& to
) const noexcept
{
    auto camera = renderer->GetCamera();

    auto& eyePos   = camera->GetEyePos();
    auto& focalPos = camera->GetFocalPos();

    std::array<double, 3> motionVector {to[0] - from[0], to[1] - from[1], to[2] - from[2]};

    camera->SetEyePos({eyePos[0] + motionVector[0], eyePos[1] + motionVector[1], eyePos[2] + motionVector[2]});
    camera->SetFocalPos({focalPos[0] + motionVector[0], focalPos[1] + motionVector[1], focalPos[2] + motionVector[2]});
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
