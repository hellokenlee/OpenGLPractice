#version 400 core

// 定义输入数据的格式
layout(isolines) in;

// V和P矩阵
uniform mat4 view;
uniform mat4 projection;

in vec3 position_TES_in[];


void main() {
	// 
	vec3 pY = (position_TES_in[2] - position_TES_in[0]) * gl_TessCoord.y;
	vec3 pX = (position_TES_in[1] - position_TES_in[0]) * gl_TessCoord.x;
	vec3 interpolatedPosition = position_TES_in[0];
	gl_Position = projection * view * vec4(interpolatedPosition + pY + pX, 1.0);
}