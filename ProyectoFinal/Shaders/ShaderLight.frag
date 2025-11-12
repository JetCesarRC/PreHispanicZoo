#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 vColor;

out vec4 color;

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS  = 2;

struct Light
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;
uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 lightDir)
{
    vec3 n = normalize(Normal);
    vec3 l = normalize(-lightDir); // dirección desde el fragmento hacia la luz

    vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;

    float diffuseFactor = max(dot(n, l), 0.0);
    vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0);

    vec4 specularColor = vec4(0.0);
    if (diffuseFactor > 0.0 && material.specularIntensity > 0.0)
    {
        vec3 viewDir = normalize(eyePosition - FragPos);
        vec3 reflectDir = reflect(-l, n);
        float specFactor = max(dot(viewDir, reflectDir), 0.0);
        if (specFactor > 0.0)
        {
            specFactor = pow(specFactor, material.shininess);
            specularColor = vec4(light.color, 1.0) *
                            material.specularIntensity * specFactor;
        }
    }

    return ambientColor + diffuseColor + specularColor;
}

vec4 CalcDirectionalLight()
{
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(PointLight pLight)
{
    vec3 lightDir = FragPos - pLight.position;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    vec4 result = CalcLightByDirection(pLight.base, lightDir);

    float attenuation =
        pLight.exponent * distance * distance +
        pLight.linear   * distance +
        pLight.constant;

    return result / attenuation;
}

vec4 CalcSpotLight(SpotLight sLight)
{
    vec3 rayDir = normalize(FragPos - sLight.base.position);
    float spotFactor = dot(rayDir, normalize(sLight.direction));

    if (spotFactor > sLight.edge)
    {
        vec4 pointColor = CalcPointLight(sLight.base);
        float intensity = 1.0 - (1.0 - spotFactor) * (1.0 / (1.0 - sLight.edge));
        return pointColor * intensity;
    }

    return vec4(0.0);
}

vec4 CalcPointLights()
{
    vec4 total = vec4(0.0);
    for (int i = 0; i < pointLightCount; i++)
    {
        total += CalcPointLight(pointLights[i]);
    }
    return total;
}

vec4 CalcSpotLights()
{
    vec4 total = vec4(0.0);
    for (int i = 0; i < spotLightCount; i++)
    {
        total += CalcSpotLight(spotLights[i]);
    }
    return total;
}

void main()
{
    vec4 lighting = CalcDirectionalLight();
    lighting += CalcPointLights();
    lighting += CalcSpotLights();

    vec4 texColor = texture(theTexture, TexCoord);
    color = texColor * vColor * lighting;
}
