#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texcoords;

out vec3 vertexNormal;
out vec3 fragPos;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(position,1.0f);
	fragPos = vec3(view * model * vec4(position,1.0f));
	vertexNormal = mat3(transpose(inverse(view*model))) * normal;
	texCoords=texcoords;
}