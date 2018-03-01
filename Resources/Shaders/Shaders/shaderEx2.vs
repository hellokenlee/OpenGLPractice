#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

uniform float offset;

out vec3 vectexColor;

void main(){
	gl_Position=vec4(position.x+offset,position.yz,1.0);
	vectexColor=color;
}