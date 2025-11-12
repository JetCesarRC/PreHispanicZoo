#include "PointLight.h"

#include <glew.h>

PointLight::PointLight()
    : Light(),
    position(0.0f, 0.0f, 0.0f),
    constant(1.0f),
    linear(0.0f),
    exponent(0.0f),
    isOn(true)
{
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
    GLfloat ambientIntensity, GLfloat diffuseIntensity,
    GLfloat xPos, GLfloat yPos, GLfloat zPos,
    GLfloat con, GLfloat lin, GLfloat exp)
    : Light(red, green, blue, ambientIntensity, diffuseIntensity),
    position(xPos, yPos, zPos),
    constant(con),
    linear(lin),
    exponent(exp),
    isOn(true)
{
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
    GLuint diffuseIntensityLocation, GLuint positionLocation,
    GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
    if (!isOn)
    {
        glUniform1f(ambientIntensityLocation, 0.0f);
        glUniform1f(diffuseIntensityLocation, 0.0f);
        return;
    }

    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}

void PointLight::SetPosition(const glm::vec3& pos)
{
    position = pos;
}

void PointLight::TurnOn() { isOn = true; }
void PointLight::TurnOff() { isOn = false; }
void PointLight::Toggle() { isOn = !isOn; }
