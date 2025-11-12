#pragma once

#include <glew.h>
#include <glm.hpp>

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat ambientIntensity, GLfloat diffuseIntensity);

    virtual ~Light() = default;

    void UseLight(GLuint ambientIntensityLocation,
        GLuint colorLocation,
        GLuint diffuseIntensityLocation);

protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
};
