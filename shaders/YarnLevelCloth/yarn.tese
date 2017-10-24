#version 400 core

// 定义输入数据的格式
layout(isolines, equal_spacing, cw) in;

// V和P矩阵
uniform mat4 view;
uniform mat4 projection;

in patch vec3 position_TES_in;

// 插值函数
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
   	// gl_TessCoord 就是PG生成的顶点在原来三角形的质心坐标
   	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}


void main() {
	// 位置插值
	vec3 interpolatedPosition = interpolate3D(position_TES_in[0], position_TES_in[1], position_TES_in[2]);
	// 设置细分后的顶点位置
	gl_Position = projection * view * vec4(interpolatedPosition, 1.0);
}