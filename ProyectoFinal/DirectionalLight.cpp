#include "DirectionalLight.h"
#include <glm.hpp>

DirectionalLight::DirectionalLight()
    : Light(), direction(0.0f, -1.0f, 0.0f)
{
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
    GLfloat ambientIntensity, GLfloat diffuseIntensity,
    GLfloat xDir, GLfloat yDir, GLfloat zDir)
    : Light(red, green, blue, ambientIntensity, diffuseIntensity),
    direction(xDir, yDir, zDir)
{
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat colorLocation,
    GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}
