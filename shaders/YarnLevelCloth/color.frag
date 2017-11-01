#version 330 core

out vec4 color;

in vec3 normal_FS_in;

uniform vec3 fragmentColor;

void main(){
	color = vec4(fragmentColor, 1.0);
}