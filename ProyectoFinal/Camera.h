#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <glfw3.h>

// ===================================================
// Cámara libre (ya existente)
// ===================================================
class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};


// ===================================================
// FollowCamera: cámara que sigue a un modelo (tercera persona)
// ===================================================
class FollowCamera {
public:
	FollowCamera(glm::vec3 offset, GLfloat turnSpeed);

	void update(const glm::vec3& targetPos, GLfloat xChange);

	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;
	float getYaw() const;

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 offset;
	GLfloat yaw;
	GLfloat turnSpeed;
};

class AerialCamera {
public:
	AerialCamera(glm::vec3 startPosition, GLfloat moveSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	glm::mat4 getViewMatrix() const;

	void setPosition(const glm::vec3& newPos);

	glm::vec3 getPosition() const;  


private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	GLfloat moveSpeed;
};



// ===================================================
// Cámara fija (elementos de interés)
// ===================================================
class FixedCamera {
public:
	FixedCamera(glm::vec3 startPosition, glm::vec3 lookTarget);
	glm::mat4 getViewMatrix() const;

private:
	glm::vec3 position;
	glm::vec3 target;
};


