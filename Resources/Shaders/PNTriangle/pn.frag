#version 400 core

in vec3 normal_FS_in;

out vec4 color;


void main(){
	color = vec4(1.0);
	//color = vec4(normal_FS_in, 1.0f);
}