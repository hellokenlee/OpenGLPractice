#version 400 core

// 定义输入数据的格式
layout(isolines, equal_spacing, ccw) in;

// V和P矩阵
uniform mat4 view;
uniform mat4 projection;

// 
in patch vec3 position_TES_in[4];


void main() {
	// 求出顶点在细分坐标后的位置
	vec3 pY = (position_TES_in[1] - position_TES_in[0]) * gl_TessCoord.y;
	vec3 pX = (position_TES_in[3] - position_TES_in[0]) * gl_TessCoord.x;
	vec3 interpolatedPosition = position_TES_in[0] + pX + pY;
	// 写入顶点在NDC的位置
	gl_Position = projection * view * vec4(interpolatedPosition, 1.0);
}