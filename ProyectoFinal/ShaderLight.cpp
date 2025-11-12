#include "ShaderLight.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader()
{
    shaderID = 0;

    uniformProjection = 0;
    uniformModel = 0;
    uniformView = 0;

    uniformEyePosition = 0;

    uniformMaterialSpecularIntensity = 0;
    uniformMaterialShininess = 0;

    uniformColor = 0;
    uniformOffset = 0;

    uniformPointLightCount = 0;
    uniformSpotLightCount = 0;
}

Shader::~Shader()
{
    ClearShader();
}

std::string Shader::ReadFile(const char* fileLocation)
{
    std::ifstream file(fileLocation, std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir el shader: " << fileLocation << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    if (vertexString.empty() || fragmentString.empty())
    {
        std::cerr << "Error: shaders vacíos al cargar." << std::endl;
        return;
    }

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    GLint result = 0;
    GLchar eLog[1024];

    shaderID = glCreateProgram();
    if (!shaderID)
    {
        std::cerr << "Error creando programa de shader." << std::endl;
        return;
    }

    // Vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertex, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error compilando shader de vértice: " << eLog << std::endl;
        glDeleteShader(vertex);
        glDeleteProgram(shaderID);
        shaderID = 0;
        return;
    }

    // Fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragment, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error compilando shader de fragmento: " << eLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(shaderID);
        shaderID = 0;
        return;
    }

    // Link
    glAttachShader(shaderID, vertex);
    glAttachShader(shaderID, fragment);
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error linkeando programa de shader: " << eLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(shaderID);
        shaderID = 0;
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Uniforms básicos
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformView = glGetUniformLocation(shaderID, "view");

    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

    uniformMaterialSpecularIntensity =
        glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformMaterialShininess =
        glGetUniformLocation(shaderID, "material.shininess");

    uniformColor = glGetUniformLocation(shaderID, "color");
    uniformOffset = glGetUniformLocation(shaderID, "toffset");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    // Direccional
    uniformDirectionalLight.uniformColor =
        glGetUniformLocation(shaderID, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity =
        glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity =
        glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection =
        glGetUniformLocation(shaderID, "directionalLight.direction");

    // Puntuales
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        std::string base = "pointLights[" + std::to_string(i) + "].";

        uniformPointLight[i].uniformColor =
            glGetUniformLocation(shaderID, (base + "base.color").c_str());
        uniformPointLight[i].uniformAmbientIntensity =
            glGetUniformLocation(shaderID, (base + "base.ambientIntensity").c_str());
        uniformPointLight[i].uniformDiffuseIntensity =
            glGetUniformLocation(shaderID, (base + "base.diffuseIntensity").c_str());
        uniformPointLight[i].uniformPosition =
            glGetUniformLocation(shaderID, (base + "position").c_str());
        uniformPointLight[i].uniformConstant =
            glGetUniformLocation(shaderID, (base + "constant").c_str());
        uniformPointLight[i].uniformLinear =
            glGetUniformLocation(shaderID, (base + "linear").c_str());
        uniformPointLight[i].uniformExponent =
            glGetUniformLocation(shaderID, (base + "exponent").c_str());
    }

    // Spotlights
    for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        std::string base = "spotLights[" + std::to_string(i) + "].";

        uniformSpotLight[i].uniformColor =
            glGetUniformLocation(shaderID, (base + "base.base.color").c_str());
        uniformSpotLight[i].uniformAmbientIntensity =
            glGetUniformLocation(shaderID, (base + "base.base.ambientIntensity").c_str());
        uniformSpotLight[i].uniformDiffuseIntensity =
            glGetUniformLocation(shaderID, (base + "base.base.diffuseIntensity").c_str());
        uniformSpotLight[i].uniformPosition =
            glGetUniformLocation(shaderID, (base + "base.position").c_str());
        uniformSpotLight[i].uniformConstant =
            glGetUniformLocation(shaderID, (base + "base.constant").c_str());
        uniformSpotLight[i].uniformLinear =
            glGetUniformLocation(shaderID, (base + "base.linear").c_str());
        uniformSpotLight[i].uniformExponent =
            glGetUniformLocation(shaderID, (base + "base.exponent").c_str());
        uniformSpotLight[i].uniformDirection =
            glGetUniformLocation(shaderID, (base + "direction").c_str());
        uniformSpotLight[i].uniformEdge =
            glGetUniformLocation(shaderID, (base + "edge").c_str());
    }
}

void Shader::UseShader()
{
    if (shaderID != 0)
        glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
    if (!dLight || shaderID == 0) return;

    dLight->UseLight(
        uniformDirectionalLight.uniformAmbientIntensity,
        uniformDirectionalLight.uniformColor,
        uniformDirectionalLight.uniformDiffuseIntensity,
        uniformDirectionalLight.uniformDirection
    );
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
    if (shaderID == 0) return;

    if (!pLight) lightCount = 0;
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    for (unsigned int i = 0; i < lightCount; i++)
    {
        pLight[i].UseLight(
            uniformPointLight[i].uniformAmbientIntensity,
            uniformPointLight[i].uniformColor,
            uniformPointLight[i].uniformDiffuseIntensity,
            uniformPointLight[i].uniformPosition,
            uniformPointLight[i].uniformConstant,
            uniformPointLight[i].uniformLinear,
            uniformPointLight[i].uniformExponent
        );
    }
}

void Shader::SetSpotLights(SpotLight* sLight, unsigned int lightCount)
{
    if (shaderID == 0) return;

    if (!sLight) lightCount = 0;
    if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);

    for (unsigned int i = 0; i < lightCount; i++)
    {
        sLight[i].UseLight(
            uniformSpotLight[i].uniformAmbientIntensity,
            uniformSpotLight[i].uniformColor,
            uniformSpotLight[i].uniformDiffuseIntensity,
            uniformSpotLight[i].uniformPosition,
            uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant,
            uniformSpotLight[i].uniformLinear,
            uniformSpotLight[i].uniformExponent,
            uniformSpotLight[i].uniformEdge
        );
    }
}
