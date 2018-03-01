#version 330 core

uniform vec3 lightColor;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main(){
	FragColor = vec4(lightColor, 1.0);
	BrightColor = vec4(lightColor, 1.0);
}