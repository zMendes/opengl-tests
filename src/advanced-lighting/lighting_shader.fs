#version 330 core

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

#define NR_POINT_LIGHTS 1

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform bool blinn;
uniform bool gamma;

//prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos,
    vec3 viewDir, bool blinn);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0.0f); //CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    result += CalcPointLight(pointLights[i], norm, fragPos, viewDir, blinn);

    //result += CalcSpotLight(spotLight, norm, fragPos, viewDir);
    //gamma correction
    if(gamma)
    result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),
        material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,
            texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,
            texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,
            texCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3
    viewDir, bool blinn) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    float spec = 0.0;
    vec3 reflectDir = reflect(-lightDir, normal);
    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 4);
    }
    else {
        spec = pow(max(dot(viewDir, reflectDir), 0.0),
            material.shininess);
    }
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation;
    if (gamma) {
        attenuation = 1.0 / (distance);
    }
    else {
        attenuation = 1.0 / (light.constant + light.linear * distance +
            light.quadratic * (distance * distance));
    }
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,
            texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,
            texCoords));
    float gamma = 2.2;
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,
            texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3
    viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),
        material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,
            texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,
            texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,
            texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}