#include "SpotLight.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <cmath>

SpotLight::SpotLight()
    : PointLight(),
    direction(0.0f, -1.0f, 0.0f),
    edge(0.0f),
    procEdge(0.0f)
{
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
    GLfloat ambientIntensity, GLfloat diffuseIntensity,
    GLfloat xPos, GLfloat yPos, GLfloat zPos,
    GLfloat dirX, GLfloat dirY, GLfloat dirZ,
    GLfloat constant, GLfloat linear, GLfloat exponent,
    GLfloat edge)
    : PointLight(red, green, blue,
        ambientIntensity, diffuseIntensity,
        xPos, yPos, zPos,
        constant, linear, exponent),
    direction(glm::normalize(glm::vec3(dirX, dirY, dirZ))),
    edge(edge),
    procEdge(std::cos(glm::radians(edge)))
{
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
    GLuint diffuseIntensityLocation, GLuint positionLocation,
    GLuint directionLocation,
    GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
    GLuint edgeLocation)
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
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);

    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);

    glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetPos(const glm::vec3& pos)
{
    position = pos;
}
