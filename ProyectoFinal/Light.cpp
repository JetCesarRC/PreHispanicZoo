#include "Light.h"

Light::Light()
    : color(1.0f, 1.0f, 1.0f),
    ambientIntensity(0.0f),
    diffuseIntensity(0.0f)
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue,
    GLfloat aIntensity, GLfloat dIntensity)
    : color(red, green, blue),
    ambientIntensity(aIntensity),
    diffuseIntensity(dIntensity)
{
}

void Light::UseLight(GLuint ambientIntensityLocation,
    GLuint colorLocation,
    GLuint diffuseIntensityLocation)
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
