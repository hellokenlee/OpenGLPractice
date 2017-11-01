#version 330 core

layout (location = 0) in vec3 position_VS_in;
layout (location = 1) in vec3 normal_VS_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal_GS_in;

void main(){
	gl_Position = projection * view * model * vec4(position_VS_in, 1.0);
	normal_GS_in = transpose(inverse(mat3(view * model))) * normal_VS_in;
}