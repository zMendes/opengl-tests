#version 330 core

struct Light {
    vec3 position;
    vec3 color;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

#define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
uniform Material material;
uniform Light lights[NR_POINT_LIGHTS];

//prototypes
void main() {
    vec3 color = texture(material.diffuse, texCoords).rgb;
    vec3 normal = normalize(normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 4; i++) {
        // diffuse
        vec3 lightDir = normalize(lights[i].position - fragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = lights[i].color * diff * color;      
        vec3 result = diffuse;        
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fragPos - lights[i].position);
        result *= 1.0 / (distance * distance);
        lighting += result;
    }
    FragColor = vec4(lighting, 1.0);
}