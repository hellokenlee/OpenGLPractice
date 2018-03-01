#version 330 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec3 vColor;

out VS_OUT{
	vec3 color;
} vs_out;

void main(){
	gl_Position = vec4(vPosition, 0.0, 1.0);
	vs_out.color= vColor;
}
