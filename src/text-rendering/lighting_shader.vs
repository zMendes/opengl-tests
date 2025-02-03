#version 330 core
in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

uniform bool inverse_normals;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;

    vec3 n = inverse_normals ? -aNormal : aNormal;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.Normal = normalize(normalMatrix * n);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}