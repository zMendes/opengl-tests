#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
const float offset = 1.0 / 300.0;

void main() {
    // Declare the offsets array
    vec2 offsets[9];
    offsets[0] = vec2(-offset, offset);  // top-left
    offsets[1] = vec2(0.0, offset);      // top-center
    offsets[2] = vec2(offset, offset);   // top-right
    offsets[3] = vec2(-offset, 0.0);     // center-left
    offsets[4] = vec2(0.0, 0.0);         // center-center
    offsets[5] = vec2(offset, 0.0);      // center-right
    offsets[6] = vec2(-offset, -offset); // bottom-left
    offsets[7] = vec2(0.0, -offset);     // bottom-center
    offsets[8] = vec2(offset, -offset);  // bottom-right

    // Declare the kernel array
    float kernel[9];
    kernel[0] = 1.0/16; kernel[1] = 2.0/16; kernel[2] = 1.0/16;
    kernel[3] = 2.0/16; kernel[4] = 4.0/16; kernel[5] = 2.0/16;
    kernel[6] = 1.0/16; kernel[7] = 2.0/16; kernel[8] = 1.0/16;

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);
}
