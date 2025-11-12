#pragma once

#include <glew.h>
#include "CommonValues.h"
#include <string>

class Texture
{
public:
    Texture();
    Texture(const char* fileLoc);

    bool LoadTexture();   // Carga textura (con o sin alpha)
    bool LoadTextureA();  // Igual, alias de compatibilidad
    void UseTexture();    // Activa y vincula la textura al shader
    void ClearTexture();  // Libera recursos

    ~Texture();

private:
    GLuint textureID;
    int width, height, bitDepth;
    const char* fileLocation;
};
