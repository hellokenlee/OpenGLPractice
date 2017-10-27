#version 330 core

out vec4 color;

uniform vec3 fragmentColor;

void main(){
	color = vec4(fragmentColor, 1.0);
}