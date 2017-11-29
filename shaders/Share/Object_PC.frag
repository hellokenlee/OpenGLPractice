#version 330 core

in vec3 color_FS_in;
out vec4 color_FS_out;

void main(){
	color_FS_out = vec4(color_FS_in, 1.0);
}