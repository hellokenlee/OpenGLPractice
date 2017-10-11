#version 400 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vPosition;

void main(){ 
	//gl_Position = projection * view * model * vec4(position,1.0);
	vPosition = vec3(projection * view * model * vec4(position,1.0));
}