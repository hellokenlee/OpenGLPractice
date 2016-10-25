#version 330 core

out vec4 color;

uniform vec3 objColor;

void main(){
	color = vec4(objColor,1.0f);
}