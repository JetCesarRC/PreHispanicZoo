#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 vColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 color;
uniform vec2 toffset;

void main()
{
    vec4 worldPos = model * vec4(pos, 1.0);

    gl_Position = projection * view * worldPos;

    TexCoord = tex + toffset;

    Normal = mat3(transpose(inverse(model))) * norm;

    FragPos = worldPos.xyz;

    vColor = vec4(color, 1.0);
}
