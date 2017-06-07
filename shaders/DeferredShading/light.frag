#version 330 core

uniform vec3 objColor;

out vec4 color;

void main(){
	color = vec4(objColor, 1.0);
}