#version 330 core
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

uniform Material material;
uniform vec3 viewPos;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

void main() {
    gPosition = fs_in.FragPos;
    gNormal = normalize(fs_in.Normal);
    gAlbedoSpec.rgb = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    gAlbedoSpec.a = texture(material.texture_specular1, fs_in.TexCoords).r;
}