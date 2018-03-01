#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fTexCoord;
out vec3 fNormal;

void main(){
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	fNormal =  transpose(inverse(mat3(model))) * vNormal;
	fTexCoord= vTexCoord;
}