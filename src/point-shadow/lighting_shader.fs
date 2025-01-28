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

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

uniform samplerCube depthMap;
uniform float far_plane;
#define NR_POINT_LIGHTS 1

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
    vec3(1, 1,  1), vec3(1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
    vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1,  0), vec3(1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
    vec3(1, 0,  1), vec3(-1,  0,  1), vec3(1,  0, -1), vec3(-1, 0, -1),
    vec3(0, 1,  1), vec3(0, -1,  1), vec3(0, -1, -1), vec3(0, 1, -1));

//prototypes
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos,
    vec3 viewDir);
float ShadowCalculation(vec3 fragPos, vec3 lightPos);

void main() {
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 result = vec3(0.0f); 
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    result += CalcPointLight(pointLights[i], norm, fs_in.fragPos, viewDir);

    //gamma correction
    result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3
    viewDir) {
    vec3 lightDir = light.position - fragPos;
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    float spec = 0.0;
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 4);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation;
    attenuation = 1.0 / (distance);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,
            fs_in.texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,
            fs_in.texCoords));
    float gamma = 2.2;
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,
            fs_in.texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = ShadowCalculation(fs_in.fragPos, light.position);
    //vec3 color = texture(diffuseTexture, fs_in.texCoords).rgb;

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    return lighting; //(ambient + diffuse + specular);
}
float ShadowCalculation(vec3 fragPos, vec3 lightPos) {
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i) {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
        shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}