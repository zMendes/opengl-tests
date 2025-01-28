#version 330 core

in vec4 fragPos;

uniform vec3 lightPos;
uniform float far_plane;
void main() {
    float lightDist = length(fragPos.xyz - lightPos);

    lightDist = lightDist/far_plane;

    gl_FragDepth = lightDist;
}