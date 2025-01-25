#version 330 core
in vec3 aPos;
in vec3 aColor;
in vec3 aNormal;
in vec2 aTexCoords;
in mat4 instanceMatrix;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main() {
    fragPos = vec3(instanceMatrix * vec4(aPos, 1.0));

    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    normal  = mat3(transpose(inverse(instanceMatrix))) * aNormal;
    texCoords = aTexCoords;
}