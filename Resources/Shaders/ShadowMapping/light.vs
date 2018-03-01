#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main(){
	gl_Position = lightSpaceMatrix * model * vec4(vPosition, 1.0);
}