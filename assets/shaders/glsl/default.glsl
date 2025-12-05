#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    FragPos     = vec3(model * vec4(aPos, 1.0));
    Normal      = mat3(transpose(inverse(model))) * aNormal;
    TexCoords   = aTexCoords;

    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform vec3 lightPosition;
uniform vec3 lightColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D shadowMap;

// --- PCF Shadow Calculation ---
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords      = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;
    float bias      = max(0.005 * (1.0 - dot(normal, lightDir)), 0.001);

    float shadow    = 0.0;
    vec2 texelSize  = 1.0 / textureSize(shadowMap, 0);
    
    // PCF 3x3 kernel
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            if(currentDepth - bias > pcfDepth)
                shadow += 1.0;
        }
    }
    shadow /= 9.0;

    // Outside shadow map
    if(projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    // --- Textures ---
    vec3 albedo     = texture(texture_diffuse1, TexCoords).rgb;
    vec3 specMap    = texture(texture_specular1, TexCoords).rgb;

    // --- Lighting Calculations ---
    vec3 norm       = normalize(Normal);
    vec3 lightDir   = normalize(lightPosition - FragPos);
    vec3 viewDir    = normalize(viewPos - FragPos);

    // Ambient
    vec3 ambient    = material.ambient * albedo;

    // Diffuse
    float diff      = max(dot(norm, lightDir), 0.0);
    vec3 diffuse    = material.diffuse * diff * albedo * lightColor;

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular   = material.specular * spec * specMap * lightColor;

    // Shadow
    float shadow    = ShadowCalculation(FragPosLightSpace, norm, lightDir);

    vec3 lighting   = ambient + (1.0 - shadow) * (diffuse + specular);

    FragColor       = vec4(lighting, 1.0);
}
