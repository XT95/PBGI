#include "camera.h"
Camera::Camera()
{}

Camera::Camera(const int &_windowWidth, const int &_windowHeight, const glm::vec3 &_position, const float &_horizontalAngle, const float &_verticalAngle,
               const float &_speed, const float &_mouseSpeed, const float &_fov)
    : windowWidth(_windowWidth), windowHeight(_windowHeight), position(_position), horizontalAngle(_horizontalAngle),
      verticalAngle(_verticalAngle), speed(_speed), mouseSpeed(_mouseSpeed), fov(_fov)
{
    computeProjection(windowWidth, windowHeight);
    right = glm::dvec3(sin(horizontalAngle - M_PI_2), 0., cos(horizontalAngle - M_PI_2));

    direction.x = cos(verticalAngle) * sin(horizontalAngle);
    direction.y = sin(verticalAngle);
    direction.z = cos(verticalAngle) * cos(horizontalAngle);

    up = glm::cross(right, direction);

    viewMatrix = glm::lookAt(position, position+direction, up);

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::verticalMovement(const int &movement)
{
    verticalAngle += mouseSpeed*movement;

    direction.x = cos(verticalAngle) * sin(horizontalAngle);
    direction.y = sin(verticalAngle);
    direction.z = cos(verticalAngle) * cos(horizontalAngle);

    up = glm::cross(right, direction);

    viewMatrix = glm::lookAt(position, position+direction, up);

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::horizontalMovement(const int &movement)
{
    horizontalAngle += mouseSpeed*movement;

    direction.x = cos(verticalAngle) * sin(horizontalAngle);
    direction.z = cos(verticalAngle) * cos(horizontalAngle);

    right.x = sin(horizontalAngle - M_PI_2);
    right.z = cos(horizontalAngle - M_PI_2);

    up = glm::cross(right, direction);

    viewMatrix = glm::lookAt(position, position+direction, up);

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::moveForward()
{
    position += direction*speed;

    viewMatrix = glm::lookAt(position, position+direction, up);

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::moveBackward()
{
    position -= direction*speed;

    viewMatrix = glm::lookAt(position, position+direction, up);

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::moveLeft()
{
    position -= right*speed;

    viewMatrix = glm::lookAt(position, position+direction, up);

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::moveRight()
{
    position += right*speed;

    viewMatrix = glm::lookAt(position, position+direction, up);

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::computeProjection(const int &_windowWidth, const int &_windowHeight)
{
    windowWidth = _windowWidth;
    windowHeight = _windowHeight;

    projectionMatrix = glm::perspective(float(M_PI*fov/180.f), float(windowWidth)/float(windowHeight), .1f, 100.f);

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::setFov(const float &_fov)
{
    if(_fov >= minFov && _fov <= maxFov)
    {
        fov = _fov;

        projectionMatrix = glm::perspective(float(M_PI*fov/180.f), float(windowWidth)/float(windowHeight), .1f, 100.f);

        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }
}

void Camera::setViewMatrix(const glm::mat4 &_viewMatrix)
{
    viewMatrix = _viewMatrix;

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::setProjectionMatrix(const glm::mat4 &_projectionMatrix)
{
    projectionMatrix = _projectionMatrix;

    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() const
{
    return viewMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
    return viewProjectionMatrix;
}

float Camera::getFov() const
{
    return fov;
}
