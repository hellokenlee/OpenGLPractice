#version 400 core

// 定义一个输出的Patch会输出多少个顶点，这里是三个
layout(vertices = 3) out;

// TCS 的Patch的输入大小由`glPatchParameteri(GL_PATCH_VERTICES, X);`指定的x
in vec3 position_TCS_in[];//长度为X
in vec3 normal_TCS_in[];

// TCS's Output
out vec3 position_TES_in[];//长度由`layout(vertices = Y) out`指定，这里是3
out vec3 normal_TES_in[];

// 摄像机位置
uniform vec3 eyePos;

// 基于与摄像机的距离划分一个3级LOD
float getTessLevel(float disA, float disB) {
	// 平均距离
	float avgDis = (disA + disB) / 2.0;
	// 这样写会不会有性能问题？分支过多？
	if(avgDis <= 2.0) {
		return 7.0;
	} else if(avgDis <= 5.0) {
		return 3.0;
	} else {
		return 1.0;
	}
}

void main() {
	// 计算顶点到摄像机距离
	float eye2VertexDis0 = distance(eyePos, position_TCS_in[0]);
	float eye2VertexDis1 = distance(eyePos, position_TCS_in[0]);
	float eye2VertexDis2 = distance(eyePos, position_TCS_in[0]);
	// 设置输出Patch的控制点
	// gl_InvocationID 表示当前调用TCS的ID
	position_TES_in[gl_InvocationID] = position_TCS_in[gl_InvocationID];
	normal_TES_in[gl_InvocationID] = normal_TCS_in[gl_InvocationID];
	//
	float avegDist = (eye2VertexDis0 + eye2VertexDis1 + eye2VertexDis2) / 3.0;
	if(avegDist > 10.0) {
		position_TES_in[gl_InvocationID] = (position_TCS_in[0] + position_TCS_in[1] + position_TCS_in[2]) / 3.0f;
	}
	// gl_TessLevelOuter 控制一个顶点的对边被细分为多少条边， 
	gl_TessLevelOuter[0] = getTessLevel(eye2VertexDis1, eye2VertexDis2);
	gl_TessLevelOuter[1] = getTessLevel(eye2VertexDis0, eye2VertexDis2);
	gl_TessLevelOuter[2] = getTessLevel(eye2VertexDis0, eye2VertexDis1);
	// gl_TessLevelInner 表示细分为同心三角形的数量
	gl_TessLevelInner[0] = 1.0;
	if(avegDist <= 2.0) {
		gl_TessLevelInner[0] = 7.0;
	} else if(avegDist <= 5.0) {
		gl_TessLevelInner[0] = 3.0;
	} else {
		gl_TessLevelInner[0] = 1.0;
	}
	//gl_TessLevelInner[0] = gl_TessLevelOuter[2];

}