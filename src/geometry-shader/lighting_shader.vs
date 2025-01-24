#version 330 core
in vec3 aPos;
in vec3 aColor;
in vec3 aNormal;
in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 fragPos;
} vs_out;

void main() {
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.normal  = mat3(transpose(inverse(model))) * aNormal;
    vs_out.texCoords = aTexCoords;
}