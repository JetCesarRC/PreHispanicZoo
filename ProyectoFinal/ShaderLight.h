#pragma once

#include <string>
#include <glew.h>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
    Shader();
    ~Shader();

    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

    void UseShader();
    void ClearShader();

    // Uniform getters principales
    GLuint GetProjectionLocation()   const { return uniformProjection; }
    GLuint GetModelLocation()        const { return uniformModel; }
    GLuint GetViewLocation()         const { return uniformView; }
    GLuint GetEyePositionLocation()  const { return uniformEyePosition; }

    GLuint GetSpecularIntensityLocation() const { return uniformMaterialSpecularIntensity; }
    GLuint GetShininessLocation()         const { return uniformMaterialShininess; }

    GLuint getColorLocation()   const { return uniformColor; }
    GLuint getOffsetLocation()  const { return uniformOffset; }

    // Luces
    void SetDirectionalLight(DirectionalLight* dLight);
    void SetPointLights(PointLight* pLight, unsigned int lightCount);
    void SetSpotLights(SpotLight* sLight, unsigned int lightCount);

private:
    GLuint shaderID;

    // Matrices
    GLuint uniformProjection;
    GLuint uniformModel;
    GLuint uniformView;

    // Cámara
    GLuint uniformEyePosition;

    // Material
    GLuint uniformMaterialSpecularIntensity;
    GLuint uniformMaterialShininess;

    // Extras
    GLuint uniformColor;
    GLuint uniformOffset;

    // Contadores
    GLuint uniformPointLightCount;
    GLuint uniformSpotLightCount;

    // Direccional
    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformDirection;
    } uniformDirectionalLight;

    // Puntuales
    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];

    // Spot
    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
        GLuint uniformDirection;
        GLuint uniformEdge;
    } uniformSpotLight[MAX_SPOT_LIGHTS];

    // Utilidades internas
    std::string ReadFile(const char* fileLocation);
    void CompileShader(const char* vertexCode, const char* fragmentCode);
};
