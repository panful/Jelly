#include "Camera.h"
#include "Logger.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Jelly;

const std::array<float, 16>& Camera::GetViewMatrix() noexcept
{
    if (m_needUpdateView)
    {
        auto viewMat = glm::lookAt(
            glm::make_vec3(m_eyePos.data()), glm::make_vec3(m_focalPos.data()), glm::make_vec3(m_viewUp.data())
        );

        std::move(glm::value_ptr(viewMat), glm::value_ptr(viewMat) + 16, m_viewMatrix.begin());

        m_needUpdateView = false;
    }

    return m_viewMatrix;
}

const std::array<float, 16>& Camera::GetProjectMatrix() noexcept
{
    if (m_needUpdateProject)
    {
        switch (m_cameraType)
        {
            case CameraType::Orthographic:
            {
                auto orthoMat = glm::ortho(-1.f, 1.f, 1.f, -1.f, 0.f, 1.f);
                std::move(glm::value_ptr(orthoMat), glm::value_ptr(orthoMat) + 16, m_projectMatrix.begin());
            }
            break;
            case CameraType::Perspective:
            {
                auto perspectiveMat = glm::perspective(glm::radians(45.f), m_aspectRatio, .1f, 100.f);
                std::move(glm::value_ptr(perspectiveMat), glm::value_ptr(perspectiveMat) + 16, m_projectMatrix.begin());
            }
            break;
            default:
            {
                Logger::GetInstance()->Warn("wrong camera type");
            }
            break;
        }

        m_needUpdateProject = false;
    }

    return m_projectMatrix;
}

const std::array<float, 3>& Camera::GetViewUp() const noexcept
{
    return m_viewUp;
}

const std::array<float, 3>& Camera::GetEyePos() const noexcept
{
    return m_eyePos;
}

const std::array<float, 3>& Camera::GetFocalPos() const noexcept
{
    return m_focalPos;
}

void Camera::SetAspectRatio(float aspectRatio) noexcept
{
    m_aspectRatio = aspectRatio;
}

void Camera::SetCameraType(CameraType cameraType) noexcept
{
    m_cameraType = cameraType;
}
