#version 400 core

// 定义输入数据的格式
layout(isolines, equal_spacing, cw) in;

// V和P矩阵
uniform mat4 view;
uniform mat4 projection;

in patch vec3 position_TES_in[4];

void main() {
	// 
	//vec3 pY = (position_TES_in[1] - position_TES_in[0]) * gl_TessCoord.y;
	//vec3 pX = (position_TES_in[3] - position_TES_in[0]) * gl_TessCoord.x;
	vec3 interpolatedPosition = vec3(0.0);
	if(gl_TessCoord.x == 0) {
		interpolatedPosition = position_TES_in[0] + vec3(0.0, gl_TessCoord.y, 0.0);
	}
	if(gl_TessCoord.x != 0) {
		interpolatedPosition = position_TES_in[3] + vec3(0.0, gl_TessCoord.y, 0.0);
	}
	gl_Position = projection * view * vec4(interpolatedPosition, 1.0);
	
}