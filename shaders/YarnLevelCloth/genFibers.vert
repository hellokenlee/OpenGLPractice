#version 400 core

layout (location = 0) in vec3 position_VS_in;

uniform mat4 model;

out vec3 position_TCS_in;

void main(){ 
	position_TCS_in = vec3(model * vec4(position_VS_in, 1.0));
}