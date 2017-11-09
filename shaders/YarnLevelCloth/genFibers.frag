#version 400 core

in vec3 color_FS_in;

out vec4 color;

void main(){
	color = vec4(color_FS_in ,1.0);
}