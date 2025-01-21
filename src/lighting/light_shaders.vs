#version 330 core
in vec4 aPos;
in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;

void main() {
    gl_Position = projection * view * model * aPos, 1.0;
    ourColor = aColor;
}