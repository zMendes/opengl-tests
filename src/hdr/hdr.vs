#version 330 core
in vec3 aPos;
in vec2 aTexCoords;

out vec2 texCoords;

void main() {
    texCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}