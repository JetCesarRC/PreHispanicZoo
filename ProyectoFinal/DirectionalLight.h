#pragma once

#include "Light.h"
#include <glm.hpp>

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir);

    void UseLight(GLfloat ambientIntensityLocation, GLfloat colorLocation,
        GLfloat diffuseIntensityLocation, GLfloat directionLocation);

    ~DirectionalLight() override = default;

private:
    glm::vec3 direction;
};
