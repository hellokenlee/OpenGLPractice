#version 400 core
// 常量们
#define PI 3.14159265354f
#define ALPHA 1.0f
#define R_PLY 2.0f
const vec3 N_YARN = vec3(0.0, 1.0, 0.0);
const vec3 B_YARN = vec3(1.0, 0.0, 0.0);
const vec3 T_YARN = vec3(0.0, 0.0, 1.0);
// 定义输入数据的格式
layout(isolines, equal_spacing, ccw) in;
// V和P矩阵
uniform mat4 view;
uniform mat4 projection;
// 
in patch vec3 position_TES_in[2];
//
out vec3 color_FS_in;

vec3 calcPlyCenter(vec3 yarnCenter, float theta_Ply) {
	// 反求 `\theta`， 转成弧度制
	float theta = (2.0f * PI * yarnCenter.z) / ALPHA;
	theta = theta * 2.0f * PI / 360.0f;
	// 计算中心相对偏移
	vec3 plyCenter = 0.5f * R_PLY * ((cos(theta_Ply + theta) * N_YARN) + (sin(theta_Ply + theta) * B_YARN));
	// 叠加
	plyCenter += yarnCenter;
	return plyCenter;
}

void main() {
	// 求出顶点在细分坐标后的位置
	vec3 deltaX = (position_TES_in[1] - position_TES_in[0]) * gl_TessCoord.x;
	vec3 pos = position_TES_in[0] + deltaX;
	color_FS_in = vec3(1.0);
	/*
	if(gl_TessCoord.y < 0.3) {
		// 
		color_FS_in = vec3(1.0, 0.0, 0.0);
		pos = calcPlyCenter(pos, 1.0f * 2.0f * PI / 3.0f);
	} else if (gl_TessCoord.y < 0.6) {
		//
		color_FS_in = vec3(0.0, 1.0, 0.0);
		pos = calcPlyCenter(pos, 2.0f * 2.0f * PI / 3.0f);
	} else {
		//
		color_FS_in = vec3(0.0, 0.0, 1.0);
		pos = calcPlyCenter(pos, 3.0f * 2.0f * PI / 3.0f);
	}
	*/
	//
	pos = calcPlyCenter(pos, gl_TessCoord.y * 2.0f * PI);
	// 写入顶点在NDC的位置
	gl_Position = projection * view * vec4(pos, 1.0);
}