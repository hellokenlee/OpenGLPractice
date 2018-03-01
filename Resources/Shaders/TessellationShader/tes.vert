#version 400 core

layout (location = 0) in vec3 position_VS_in;
layout (location = 1) in vec3 normal_VS_in;

uniform mat4 model;

out vec3 position_TCS_in;
out vec3 normal_TCS_in;

void main(){ 
	// 因为只是控制点，没必要在这乘VP矩阵，直接把局部坐标传到后面即可
	// 后面生成的顶点才需要乘VP
	// 乘上Model矩阵是为了后面计算和视点的距离(如果不做LOD的话Model都不需要乘)
	position_TCS_in = vec3(model * vec4(position_VS_in, 1.0));
	normal_TCS_in = transpose(inverse(mat3(model))) * normal_VS_in;
}