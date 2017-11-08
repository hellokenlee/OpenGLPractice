#version 400 core

// 定义输入数据的格式
layout(isolines, equal_spacing, ccw) in;

// V和P矩阵
uniform mat4 view;
uniform mat4 projection;

// 
in patch vec3 position_TES_in[];
//
out vec3 color_FS_in;

vec3 calcPlyCenter(vec3 yarnCenter, float theta_Ply) {
	
}

void main() {
	// 求出顶点在细分坐标后的位置
	vec3 deltaX = (position_TES_in[1] - position_TES_in[0]) * gl_TessCoord.x;
	vec3 pos = position_TES_in[0] + deltaX;
	color_FS_in = vec3(1.0);
	// 根据公式算出
	if(gl_TessCoord.y < 0.3) {
		// 
		color_FS_in = vec3(1.0, 0.0, 0.0);
	} else if (gl_TessCoord.y < 0.6) {
		//
		color_FS_in = vec3(0.0, 1.0, 0.0);
	} else {
		//

		color_FS_in = vec3(0.0, 0.0, 1.0);
	}
	// 写入顶点在NDC的位置
	//gl_Position = projection * view * vec4(interpolatedPosition, 1.0);
}