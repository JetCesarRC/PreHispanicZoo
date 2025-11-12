#pragma once

#include "Light.h"
#include <glm.hpp>

class PointLight : public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat con, GLfloat lin, GLfloat exp);

    void UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    void SetPosition(const glm::vec3& pos);
    void TurnOn();
    void TurnOff();
    void Toggle();
    bool IsOn() const { return isOn; }

    ~PointLight() override = default;

protected:
    glm::vec3 position;
    GLfloat constant;
    GLfloat linear;
    GLfloat exponent;
    bool isOn;
};
