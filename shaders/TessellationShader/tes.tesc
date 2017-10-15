#version 400 core

// 定义一个输出的Patch会输出多少个顶点，这里是三个
layout(vertices = 3) out;

in vec3 vPosition[];
out vec3 tcPosition[];

uniform vec3 eyePos;

float getTessLevel(float disA, disB) {
	float avgDis = (disA + disB) / 2.0;
	if(avgDis <= 2.0) {
		return 10.0;
	} else if(avgDis <= 5.0) {
		return 7.0;
	} else {
		return 3.0;
	}
}

void main() {
	// 设置输出Patch的控制点
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
	// 计算顶点到摄像机距离
	float eye2VertexDis0 = distance(eyePos, vPosition[0]);
	float eye2VertexDis1 = distance(eyePos, vPosition[1]);
	float eye2VertexDis2 = distance(eyePos, vPosition[2]);
	// 计算细分等级
	gl_TessLevelOuter[0] = getTessLevel(eye2VertexDis1, eye2VertexDis2);
	gl_TessLevelOuter[1] = getTessLevel(eye2VertexDis0, eye2VertexDis2);
	gl_TessLevelOuter[2] = getTessLevel(eye2VertexDis0, eye2VertexDis1);
	//
	gl_TessLevelInner[0] = gl_TessLevelOuter[2];

}