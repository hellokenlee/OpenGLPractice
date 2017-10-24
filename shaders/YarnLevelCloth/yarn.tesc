#version 400 core

// 以Patch形式输出，所以为1
layout(vertices = 1) out;

in vec3 position_TCS_in[];//Len = 2

// TCS's Output
out patch vec3 position_TES_in;

void main() {
	
	gl_TessLevelOuter[0] = 3.0;
	
	gl_TessLevelInner[0] = 4.0;
}