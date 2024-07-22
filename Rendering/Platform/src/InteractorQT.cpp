#include "InteractorQT.h"
#include "InteractorStyleTrackballCamera.h"
#include "WindowQT.h"
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <any>

using namespace Jelly;

void InteractorQT::Start() noexcept
{
    if (!m_interactorStyle)
    {
        m_interactorStyle = std::make_shared<InteractorStyleTrackballCamera>();
        m_interactorStyle->SetInteractor(weak_from_this());
    }

    if (!m_window->HasInitialized())
    {
        m_window->Render();
    }

    auto qWindow = std::any_cast<WindowQT*>(m_window->GetNativeWindow());

    qWindow->SetEventAdapter([this](QEvent* event) {
        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::MouseButtonDblClick || event->type() == QEvent::MouseMove)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

            this->m_mousePosition = {static_cast<int>(mouseEvent->pos().x()), static_cast<int>(mouseEvent->pos().y())};

            switch (event->type())
            {
                case QEvent::MouseMove:
                {
                    this->m_interactorStyle->MouseMoveEvent();
                }
                break;
                case QEvent::MouseButtonPress:
                {
                    switch (mouseEvent->button())
                    {
                        case Qt::LeftButton:
                            this->m_interactorStyle->LeftButtonPressEvent();
                            break;
                        case Qt::RightButton:
                            this->m_interactorStyle->RightButtonPressEvent();
                            break;
                        case Qt::MiddleButton:
                            this->m_interactorStyle->MiddleButtonPressEvent();
                            break;
                        default:
                            break;
                    }
                }
                break;
                case QEvent::MouseButtonRelease:
                {
                    switch (mouseEvent->button())
                    {
                        case Qt::LeftButton:
                            this->m_interactorStyle->LeftButtonReleaseEvent();
                            break;
                        case Qt::RightButton:
                            this->m_interactorStyle->RightButtonReleaseEvent();
                            break;
                        case Qt::MiddleButton:
                            this->m_interactorStyle->MiddleButtonReleaseEvent();
                            break;
                        default:
                            break;
                    }
                }
                break;
                default:
                    break;
            }
        }

        if (event->type() == QEvent::Wheel)
        {
            QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);

            static constexpr int threshold {120};

            double horizontalDelta = wheelEvent->angleDelta().x();
            double verticalDelta   = wheelEvent->angleDelta().y();
            auto accumulatedDelta  = static_cast<int>(verticalDelta + horizontalDelta);

            if (accumulatedDelta >= threshold && verticalDelta != 0.0)
            {
                this->m_interactorStyle->MouseWheelForwardEvent();
            }
            else if (accumulatedDelta <= -threshold && verticalDelta != 0.0)
            {
                this->m_interactorStyle->MouseWheelBackwardEvent();
            }
        }
    });
}
