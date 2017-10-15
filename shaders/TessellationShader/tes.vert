#version 400 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vPosition;

void main(){ 
	//gl_Position = projection * view * model * vec4(position,1.0);
	//vPosition = vec3(projection * view * model * vec4(position,1.0));
	// 因为只是控制点，没必要在这乘VP矩阵，直接把局部坐标传到后面即可
	// 后面生成的顶点才需要乘VP
	// 乘上Model矩阵是为了后面计算和视点的距离
	vPosition = vec3(model * vec4(position, 1.0));
}