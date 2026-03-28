#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float k_c;
    float k_l;
    float k_q;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    float cutOff;
    float outerCutOff;
};

#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS  4

uniform vec3      viewPos;
uniform vec3      globalAmbient;
uniform Material  material;
uniform DirLight  dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight  spotLights[NR_SPOT_LIGHTS];

// Texture support
uniform sampler2D texture1;
uniform bool      useTexture;
uniform float     texRepeat;
uniform float     textureBlend;

// ---- lighting functions ----

vec3 CalcDirLight(DirLight light, vec3 N, vec3 V, vec3 matAmb, vec3 matDiff, vec3 matSpec)
{
    vec3 L    = normalize(-light.direction);
    float d   = max(dot(N, L), 0.0);
    vec3 R    = reflect(-L, N);
    float s   = pow(max(dot(V, R), 0.0), material.shininess);
    return ( light.ambient  * matAmb
           + light.diffuse  * d * matDiff
           + light.specular * s * matSpec );
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 P, vec3 V, vec3 matAmb, vec3 matDiff, vec3 matSpec)
{
    vec3  L   = normalize(light.position - P);
    float d   = max(dot(N, L), 0.0);
    vec3  R   = reflect(-L, N);
    float s   = pow(max(dot(V, R), 0.0), material.shininess);
    float dist = length(light.position - P);
    float att  = 1.0 / (light.k_c + light.k_l * dist + light.k_q * dist * dist);
    return att * ( light.ambient  * matAmb
                 + light.diffuse  * d * matDiff
                 + light.specular * s * matSpec );
}

vec3 CalcSpotLight(SpotLight light, vec3 N, vec3 P, vec3 V, vec3 matAmb, vec3 matDiff, vec3 matSpec)
{
    vec3  L    = normalize(light.position - P);
    float d    = max(dot(N, L), 0.0);
    vec3  R    = reflect(-L, N);
    float s    = pow(max(dot(V, R), 0.0), material.shininess);
    float dist = length(light.position - P);
    float att  = 1.0 / (light.k_c + light.k_l * dist + light.k_q * dist * dist);
    float theta = dot(L, normalize(-light.direction));
    // Binary cutoff: full intensity inside cone, zero outside
    float inten = theta > light.cutOff ? 1.0 : 0.0;
    return att * ( light.ambient  * matAmb
                 + inten * light.diffuse  * d * matDiff
                 + inten * light.specular * s * matSpec );
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    // Determine effective material colors (optionally modulated by texture)
    vec3 matAmb  = material.ambient;
    vec3 matDiff = material.diffuse;
    vec3 matSpec = material.specular;

    if (useTexture) {
        vec2 tc = TexCoord * texRepeat;
        vec3 texColor = texture(texture1, tc).rgb;
        float blend = clamp(textureBlend, 0.0, 1.0);
        matAmb  = mix(material.ambient, texColor * 0.4, blend);
        matDiff = mix(material.diffuse, texColor, blend);
        // specular stays from material
    }

    vec3 result = globalAmbient * matAmb;

    result += CalcDirLight(dirLight, N, V, matAmb, matDiff, matSpec);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], N, FragPos, V, matAmb, matDiff, matSpec);

    for (int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], N, FragPos, V, matAmb, matDiff, matSpec);

    result += material.emissive;

    FragColor = vec4(result, 1.0);
}
