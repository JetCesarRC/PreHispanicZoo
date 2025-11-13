#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	if (keys[GLFW_KEY_Z])
	{ 
		position += worldUp * velocity;  // subir
	}

	if (keys[GLFW_KEY_X])  // bajar
	{
		position -= worldUp * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}

// ===================================================
// Implementación FollowCamera
// ===================================================
FollowCamera::FollowCamera(glm::vec3 offset, GLfloat turnSpeed)
	: offset(offset), turnSpeed(turnSpeed), yaw(0.0f) {
}

void FollowCamera::update(const glm::vec3& targetPos, GLfloat xChange)
{
	yaw += xChange * turnSpeed;

	if (yaw > 360.0f) yaw -= 360.0f;
	if (yaw < 0.0f) yaw += 360.0f;

	position.x = targetPos.x - offset.z * sin(glm::radians(yaw));
	position.z = targetPos.z - offset.z * cos(glm::radians(yaw));
	position.y = targetPos.y + offset.y;

	glm::vec3 lookTarget = targetPos + glm::vec3(0.0f, 5.0f, 0.0f);
	front = glm::normalize(lookTarget - position);
	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(right, front));
}

glm::mat4 FollowCamera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 FollowCamera::getPosition() const
{
	return position;
}

float FollowCamera::getYaw() const
{
	return yaw;
}

// ======================================================
// Implementación AerialCamera
// ======================================================
AerialCamera::AerialCamera(glm::vec3 startPos, float speed)
	: position(startPos), moveSpeed(speed) {
	front = glm::vec3(0.0f, -1.0f, 0.0f); // mira hacia abajo
	up = glm::vec3(0.0f, 0.0f, -1.0f);    // "arriba" invertido para que mire bien
}

void AerialCamera::keyControl(bool* keys, GLfloat deltaTime) {
	float velocity = moveSpeed * deltaTime;
	if (keys[GLFW_KEY_W]) position.z -= velocity; // adelante
	if (keys[GLFW_KEY_S]) position.z += velocity; // atrás
	if (keys[GLFW_KEY_A]) position.x -= velocity; // izquierda
	if (keys[GLFW_KEY_D]) position.x += velocity; // derecha
}

glm::mat4 AerialCamera::getViewMatrix() const {
	return glm::lookAt(position, position + front, up);
}

void AerialCamera::setPosition(const glm::vec3& newPos) {
	position = newPos;
}

glm::vec3 AerialCamera::getPosition() const {
	return position;
}



// ======================================================
// Implementación FixedCamera
// ======================================================
FixedCamera::FixedCamera(glm::vec3 pos, glm::vec3 tgt)
	: position(pos), target(tgt) {
}

glm::mat4 FixedCamera::getViewMatrix() const {
	return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}




