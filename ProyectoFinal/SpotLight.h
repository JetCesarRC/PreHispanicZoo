#pragma once

#include "PointLight.h"
#include <glm.hpp>

class SpotLight : public PointLight
{
public:
    SpotLight();
    SpotLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat dirX, GLfloat dirY, GLfloat dirZ,
        GLfloat constant, GLfloat linear, GLfloat exponent,
        GLfloat edge);

    void UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);

    void SetPos(const glm::vec3& pos);

    ~SpotLight() override = default;

private:
    glm::vec3 direction;
    GLfloat edge;
    GLfloat procEdge;
};
