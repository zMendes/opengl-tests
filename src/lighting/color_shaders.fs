#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
out vec4 FragColor;

in vec3 ourColor;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

void main() {
    //ambient
    vec3 ambient = light.ambient * material.ambient ;

    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    //specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm); //negate lightDir because its pointing to the light instead
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;

    vec3 result = (ambient + diffuse + specular) * ourColor;
    FragColor = vec4(result, 1.0);
}