#version 400 core

#define TESSELLATION_LEVEL 10.0

// 每个Patch只输出一个控制点
layout(vertices = 1) out;

// TCS 的Patch的输入大小由`glPatchParameteri(GL_PATCH_VERTICES, X);`指定的x
in vec3 position_TCS_in[];//长度为X
in vec3 normal_TCS_in[];

// 输出结构体
struct OutputPatch{
	vec3 position_B030;
	vec3 position_B021;
	vec3 position_B012;

	vec3 position_B003;
	vec3 position_B102;
	vec3 position_B201;

	vec3 position_B300;
	vec3 position_B210;
	vec3 position_B120;

	vec3 position_B111;

	vec3 normal[3];
};

// 声明输出
out patch OutputPatch patch_TES_in;

// 投影
vec3 projection2Plane(vec3 point, vec3 planePoint, vec3 planeNormal) {
	vec3 v = point - planePoint;
	float len = dot(v, planeNormal);
	vec3 d = len * planeNormal;
	return (point - d);
}

// 把一个三个点的输入变成10个控制点
void calcPositions() {
	// 三个顶点是不变的
	patch_TES_in.position_B030 = position_TCS_in[0];
	patch_TES_in.position_B003 = position_TCS_in[1];
	patch_TES_in.position_B300 = position_TCS_in[2];
	// 计算出顶点对应的边的向量
	vec3 edgeB030 = patch_TES_in.position_B300 - patch_TES_in.position_B003;
	vec3 edgeB003 = patch_TES_in.position_B030 - patch_TES_in.position_B300;
	vec3 edgeB300 = patch_TES_in.position_B003 - patch_TES_in.position_B030;
	// 生成边上的中间点
		// edge300上的两个点
	patch_TES_in.position_B021 = patch_TES_in.position_B030 + (edgeB300 / 3.0);
	patch_TES_in.position_B012 = patch_TES_in.position_B030 + (edgeB300 * 2.0 / 3.0);
		// edge030上的两个点
	patch_TES_in.position_B102 = patch_TES_in.position_B003 + (edgeB030 / 3.0);
	patch_TES_in.position_B201 = patch_TES_in.position_B003 + (edgeB030 * 2.0 / 3.0);
		// edge003上的两个点
	patch_TES_in.position_B210 = patch_TES_in.position_B300 + (edgeB003 / 3.0);
	patch_TES_in.position_B120 = patch_TES_in.position_B300 + (edgeB003 * 2.0 / 3.0);
	// 把其投影到最近的三角顶点及其法线定义的平面上
	patch_TES_in.position_B021 =  projection2Plane(patch_TES_in.position_B021,
													patch_TES_in.position_B030,
													normal_TCS_in[0]);
	patch_TES_in.position_B012 =  projection2Plane(patch_TES_in.position_B012,
													patch_TES_in.position_B003,
													normal_TCS_in[1]);
	patch_TES_in.position_B102 =  projection2Plane(patch_TES_in.position_B102,
													patch_TES_in.position_B003,
													normal_TCS_in[1]);
	patch_TES_in.position_B201 =  projection2Plane(patch_TES_in.position_B201,
													patch_TES_in.position_B300,
													normal_TCS_in[2]);
	patch_TES_in.position_B210 =  projection2Plane(patch_TES_in.position_B210,
													patch_TES_in.position_B300,
													normal_TCS_in[2]);
	patch_TES_in.position_B120 =  projection2Plane(patch_TES_in.position_B120,
													patch_TES_in.position_B030,
													normal_TCS_in[0]);
	// 最后计算出中心点
	vec3 center = (patch_TES_in.position_B030 + patch_TES_in.position_B003  + patch_TES_in.position_B300);
	patch_TES_in.position_B111 = (patch_TES_in.position_B021 + patch_TES_in.position_B012 + patch_TES_in.position_B102 
								+ patch_TES_in.position_B201 + patch_TES_in.position_B210 + patch_TES_in.position_B120) / 6.0;
	patch_TES_in.position_B111 += (patch_TES_in.position_B111 - center) / 2.0;
}

//
void main() {
	// 直接传递法向量
	for(int i = 0; i < 3; ++i) {
		patch_TES_in.normal[i] = normal_TCS_in[i];
	}
	//
	calcPositions();
	// gl_TessLevelOuter 控制一个顶点的对边被细分为多少条边， 
	gl_TessLevelOuter[0] = TESSELLATION_LEVEL;
	gl_TessLevelOuter[1] = TESSELLATION_LEVEL;
	gl_TessLevelOuter[2] = TESSELLATION_LEVEL;
	// gl_TessLevelInner 表示细分为同心三角形的数量
	gl_TessLevelInner[0] = TESSELLATION_LEVEL;
}