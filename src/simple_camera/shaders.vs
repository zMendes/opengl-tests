#version 330 core
in vec4 aPos;
in vec3 aColor;
in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{

    gl_Position = projection * view * model * aPos, 1.0;
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}