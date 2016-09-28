#version 330 core

in vec3 vectexColor;

out vec4 color;

void main(){
	color=vec4(vectexColor,1.0);
}