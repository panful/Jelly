#include "InteractorStyle.h"
#include "Camera.h"
#include "Interactor.h"
#include "Logger.h"
#include "Renderer.h"
#include "Window.h"
#include <cmath>

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

void InteractorStyle::Dolly(double factor) const noexcept
{
    if (factor <= 0)
    {
        return;
    }

    if (auto render = m_currentRenderer.lock())
    {
        auto camera = render->GetCamera();
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
                auto d = std::hypot(x, y, z);

                x /= d;
                y /= d;
                z /= d;
                d /= factor;

                camera->SetEyePos({focalPos[0] - d * x, focalPos[1] - d * y, focalPos[2] - d * z});
            }
            break;
            default:
            {
                Logger::GetInstance()->Warn("wrong camera type");
            }
            break;
        }
    }
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
