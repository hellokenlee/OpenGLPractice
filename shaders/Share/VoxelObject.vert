#version 330 core

layout (location = 0) in vec3 position_VS_in;
layout (location = 1) in vec3 normal_VS_in;
layout (location = 2) in vec2 texCoord_VS_in;
layout (location = 3) in mat4 model_VS_in;

out vec2 texCoord_FS_in;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model_VS_in * vec4(position_VS_in, 1.0);
    texCoord_FS_in = texCoord_VS_in;
}