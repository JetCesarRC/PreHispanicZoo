#include "Camera.h"
#include <glfw3.h>
#include <gtc/matrix_transform.hpp>

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    moveSpeed = 3.0f;
    turnSpeed = 0.1f;

    update();
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch,
    GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
    GLfloat velocity = moveSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
        position += front * velocity;

    if (keys[GLFW_KEY_S])
        position -= front * velocity;

    if (keys[GLFW_KEY_A])
        position -= right * velocity;

    if (keys[GLFW_KEY_D])
        position += right * velocity;

    if (keys[GLFW_KEY_Z])
        position.y += velocity;

    if (keys[GLFW_KEY_X])
        position.y -= velocity;
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    update();
}

glm::mat4 Camera::calculateViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition() const
{
    return position;
}

glm::vec3 Camera::getCameraDirection() const
{
    return glm::normalize(front);
}

void Camera::update()
{
    glm::vec3 newFront;
    newFront.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    newFront.y = sinf(glm::radians(pitch));
    newFront.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
