#pragma once

#include <glew.h>

class Material
{
public:
    Material();
    Material(GLfloat specularIntensity, GLfloat shininess);

    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) const;

private:
    GLfloat specularIntensity;
    GLfloat shininess;
};
