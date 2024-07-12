#include "Camera.h"
#include "Logger.h"
#include <cmath>
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
    if (m_viewUp != viewUp)
    {
        m_viewUp     = viewUp;
        m_needUpdate = true;
    }
}

const std::array<double, 3>& Camera::GetEyePos() const noexcept
{
    return m_eyePos;
}

void Camera::SetEyePos(const std::array<double, 3>& eyePos) noexcept
{
    if (m_eyePos != eyePos)
    {
        m_eyePos     = eyePos;
        m_needUpdate = true;
    }
}

const std::array<double, 3>& Camera::GetFocalPos() const noexcept
{
    return m_focalPos;
}

void Camera::SetFocalPos(const std::array<double, 3>& focalPos) noexcept
{
    if (m_focalPos != focalPos)
    {
        m_focalPos   = focalPos;
        m_needUpdate = true;
    }
}

void Camera::Pitch(double angle) noexcept
{
    glm::dvec3 viewUp   = glm::make_vec3(m_viewUp.data());
    glm::dvec3 eyePos   = glm::make_vec3(m_eyePos.data());
    glm::dvec3 focalPos = glm::make_vec3(m_focalPos.data());
    glm::dvec3 axis     = glm::normalize(glm::cross(focalPos - eyePos, viewUp));

    glm::dmat4 transform(1.);
    transform = glm::translate(transform, eyePos);
    transform = glm::rotate(transform, angle, axis);
    transform = glm::translate(transform, -eyePos);

    focalPos = glm::dvec3(transform * glm::dvec4(focalPos, 1.));
    viewUp   = glm::normalize(glm::cross(axis, focalPos - eyePos));

    m_focalPos = {focalPos[0], focalPos[1], focalPos[2]};
    m_viewUp   = {viewUp[0], viewUp[1], viewUp[2]};

    m_needUpdate = true;
}

void Camera::Yaw(double angle) noexcept
{
    glm::dvec3 viewUp   = glm::make_vec3(m_viewUp.data());
    glm::dvec3 eyePos   = glm::make_vec3(m_eyePos.data());
    glm::dvec3 focalPos = glm::make_vec3(m_focalPos.data());
    glm::dvec3 axis     = glm::normalize(viewUp);

    glm::dmat4 transform(1.);
    transform = glm::translate(transform, eyePos);
    transform = glm::rotate(transform, angle, axis);
    transform = glm::translate(transform, -eyePos);

    focalPos   = glm::dvec3(transform * glm::dvec4(focalPos, 1.));
    m_focalPos = {focalPos[0], focalPos[1], focalPos[2]};

    m_needUpdate = true;
}

void Camera::Roll(double angle) noexcept
{
    glm::dvec3 viewUp   = glm::make_vec3(m_viewUp.data());
    glm::dvec3 eyePos   = glm::make_vec3(m_eyePos.data());
    glm::dvec3 focalPos = glm::make_vec3(m_focalPos.data());
    glm::dvec3 axis     = glm::normalize(focalPos - eyePos);

    glm::dmat4 transform(1.);
    transform = glm::translate(transform, eyePos);
    transform = glm::rotate(transform, angle, axis);
    transform = glm::translate(transform, -eyePos);

    viewUp   = glm::dvec3(transform * glm::dvec4(viewUp, 1.));
    m_viewUp = {viewUp[0], viewUp[1], viewUp[2]};

    m_needUpdate = true;
}

void Camera::Azimuth(double angle) noexcept
{
    glm::dvec3 viewUp   = glm::make_vec3(m_viewUp.data());
    glm::dvec3 eyePos   = glm::make_vec3(m_eyePos.data());
    glm::dvec3 focalPos = glm::make_vec3(m_focalPos.data());
    glm::dvec3 axis     = glm::normalize(viewUp);

    glm::dmat4 transform(1.);
    transform = glm::translate(transform, focalPos);
    transform = glm::rotate(transform, angle, axis);
    transform = glm::translate(transform, -focalPos);

    eyePos   = glm::dvec3(transform * glm::dvec4(eyePos, 1.));
    m_eyePos = {eyePos[0], eyePos[1], eyePos[2]};

    m_needUpdate = true;
}

void Camera::Elevation(double angle) noexcept
{
    glm::dvec3 viewUp   = glm::make_vec3(m_viewUp.data());
    glm::dvec3 eyePos   = glm::make_vec3(m_eyePos.data());
    glm::dvec3 focalPos = glm::make_vec3(m_focalPos.data());
    glm::dvec3 axis     = glm::normalize(glm::cross(focalPos - eyePos, viewUp));

    glm::dmat4 transform(1.);
    transform = glm::translate(transform, focalPos);
    transform = glm::rotate(transform, angle, axis);
    transform = glm::translate(transform, -focalPos);

    eyePos = glm::dvec3(transform * glm::dvec4(eyePos, 1.));
    viewUp = glm::normalize(glm::cross(axis, focalPos - eyePos));

    m_eyePos = {eyePos[0], eyePos[1], eyePos[2]};
    m_viewUp = {viewUp[0], viewUp[1], viewUp[2]};

    m_needUpdate = true;
}

void Camera::SetViewAngle(double viewAngle) noexcept
{
    if (m_viewAngle != viewAngle)
    {
        m_viewAngle  = viewAngle;
        m_needUpdate = true;
    }
}

double Camera::GetViewAngle() const noexcept
{
    return m_viewAngle;
}

void Camera::SetClipRange(const std::array<double, 2>& clipRange) noexcept
{
    if (m_clipRange != clipRange)
    {
        m_clipRange  = clipRange;
        m_needUpdate = true;
    }
}

const std::array<double, 2>& Camera::GetClipRange() const noexcept
{
    return m_clipRange;
}

void Camera::SetOrthographicScale(double orthographicScale) noexcept
{
    if (m_orthographicScale != orthographicScale)
    {
        m_orthographicScale = orthographicScale;
        m_needUpdate        = true;
    }
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
    if (m_cameraType != cameraType)
    {
        m_cameraType = cameraType;
        m_needUpdate = true;
    }
}

CameraType Camera::GetCameraType() const noexcept
{
    return m_cameraType;
}

std::array<double, 3> Camera::NDCToWorld(const std::array<double, 3>& ndcPoint) const noexcept
{
    glm::dvec4 ndc {ndcPoint[0], ndcPoint[1], ndcPoint[2], 1.};

    glm::dmat4 viewMat = glm::make_mat4(m_viewMatrix.data());
    glm::dmat4 projMat = glm::make_mat4(m_projectMatrix.data());

    glm::dvec4 worldPos = glm::inverse(viewMat) * glm::inverse(projMat) * ndc;

    return {worldPos.x / worldPos.w, worldPos.y / worldPos.w, worldPos.z / worldPos.w};
}

std::array<double, 3> Camera::WorldToView(const std::array<double, 3>& worldPoint) const noexcept
{
    glm::dvec4 world {worldPoint[0], worldPoint[1], worldPoint[2], 1.};

    glm::dmat4 viewMat = glm::make_mat4(m_viewMatrix.data());
    glm::dmat4 projMat = glm::make_mat4(m_projectMatrix.data());

    glm::dvec4 viewPos = projMat * viewMat * world;

    return {viewPos.x / viewPos.w, viewPos.y / viewPos.w, viewPos.z / viewPos.w};
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
            auto tmp    = std::tan(m_viewAngle / 2.);
            auto width  = m_clipRange[0] * tmp * m_aspectRatio;
            auto height = m_clipRange[0] * tmp;

            auto left   = static_cast<float>(width * m_viewRange[0]);
            auto right  = static_cast<float>(width * m_viewRange[1]);
            auto top    = static_cast<float>(height * m_viewRange[2]);
            auto bottom = static_cast<float>(height * m_viewRange[3]);
            auto near   = static_cast<float>(m_clipRange[0]);
            auto far    = static_cast<float>(m_clipRange[1]);

            auto frustum = glm::frustum(left, right, bottom, top, near, far);
            dMat4TofArr16(frustum, m_projectMatrix);
        }
        break;
        default:
        {
            Logger::GetInstance()->Warn("wrong camera type");
        }
        break;
    }
}
