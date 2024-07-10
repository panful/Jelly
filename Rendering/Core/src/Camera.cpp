#include "Camera.h"
#include "Logger.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Jelly;

const std::array<float, 16>& Camera::GetViewMatrix() noexcept
{
    if (m_needUpdate)
    {
        Update();
        m_needUpdate = false;
    }

    return m_viewMatrix;
}

const std::array<float, 16>& Camera::GetProjectMatrix() noexcept
{
    if (m_needUpdate)
    {
        Update();
        m_needUpdate = false;
    }

    return m_projectMatrix;
}

const std::array<double, 3>& Camera::GetViewUp() const noexcept
{
    return m_viewUp;
}

void Camera::SetViewUp(const std::array<double, 3>& viewUp) noexcept
{
    m_viewUp = viewUp;
}

const std::array<double, 3>& Camera::GetEyePos() const noexcept
{
    return m_eyePos;
}

void Camera::SetEyePos(const std::array<double, 3>& eyePos) noexcept
{
    m_eyePos = eyePos;
}

const std::array<double, 3>& Camera::GetFocalPos() const noexcept
{
    return m_focalPos;
}

void Camera::SetFocalPos(const std::array<double, 3>& focalPos) noexcept
{
    m_focalPos = focalPos;
}

void Camera::SetViewAngle(double viewAngle) noexcept
{
    m_viewAngle = viewAngle;
}

double Camera::GetViewAngle() const noexcept
{
    return m_viewAngle;
}

void Camera::SetClipRange(const std::array<double, 2>& clipRange) noexcept
{
    m_clipRange = clipRange;
}

void Camera::SetOrthographicScale(double orthographicScale) noexcept
{
    m_orthographicScale = orthographicScale;
}

double Camera::GetOrthographicScale() const noexcept
{
    return m_orthographicScale;
}

std::array<double, 3> Camera::GetViewPlaneNormal() const noexcept
{
    auto vpn =
        glm::normalize(glm::vec3 {m_eyePos[0] - m_focalPos[0], m_eyePos[1] - m_focalPos[1], m_eyePos[2] - m_focalPos[2]}
        );

    return {vpn[0], vpn[1], vpn[2]};
}

std::array<double, 3> Camera::GetProjectionDirection() const noexcept
{
    auto pd =
        glm::normalize(glm::vec3 {m_focalPos[0] - m_eyePos[0], m_focalPos[1] - m_eyePos[1], m_focalPos[2] - m_eyePos[2]}
        );

    return {pd[0], pd[1], pd[2]};
}

void Camera::SetAspectRatio(double aspectRatio) noexcept
{
    if (m_aspectRatio != aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        m_needUpdate  = true;
    }
}

void Camera::SetCameraType(CameraType cameraType) noexcept
{
    m_cameraType = cameraType;
}

void Camera::Update()
{
    auto dMat4TofArr16 = [](const glm::dmat4& from, std::array<float, 16>& to) {
        for (auto i = 0u; i < 4u; ++i)
        {
            for (auto j = 0u; j < 4u; ++j)
            {
                to[i * 4 + j] = static_cast<float>(from[i][j]);
            }
        }
    };

    auto viewMat = glm::lookAt(
        glm::make_vec3(m_eyePos.data()), glm::make_vec3(m_focalPos.data()), glm::make_vec3(m_viewUp.data())
    );

    dMat4TofArr16(viewMat, m_viewMatrix);

    switch (m_cameraType)
    {
        case CameraType::Orthographic:
        {
            auto left   = static_cast<float>(m_orthographicScale * m_viewRange[0] * m_aspectRatio);
            auto right  = static_cast<float>(m_orthographicScale * m_viewRange[1] * m_aspectRatio);
            auto top    = static_cast<float>(m_orthographicScale * m_viewRange[2]);
            auto bottom = static_cast<float>(m_orthographicScale * m_viewRange[3]);
            auto zNear  = static_cast<float>(m_clipRange[0]);
            auto zFar   = static_cast<float>(m_clipRange[1]);

            auto orthoMat = glm::ortho(left, right, bottom, top, zNear, zFar);
            dMat4TofArr16(orthoMat, m_projectMatrix);
        }
        break;
        case CameraType::Perspective:
        {
            auto perspectiveMat = glm::perspective(glm::radians(45.f), static_cast<float>(m_aspectRatio), .1f, 100.f);
            dMat4TofArr16(perspectiveMat, m_projectMatrix);
        }
        break;
        default:
        {
            Logger::GetInstance()->Warn("wrong camera type");
        }
        break;
    }
}
