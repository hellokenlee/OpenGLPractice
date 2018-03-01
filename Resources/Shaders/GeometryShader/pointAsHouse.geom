#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT{
	vec3 color;
} gs_in[];
// 尽管当前我们只有一个顶点，但是总是会以数组形式传入几何着色器。 
// 考虑一下顶点着色器处理的是三角形的情况，就会有3个数组传入。

out vec3 fColor;

void main(){
	fColor = gs_in[0].color;

	gl_Position = gl_in[0].gl_Position+vec4(-0.1, -0.1, 0.0, 0.0); // 左下角
	EmitVertex();

	gl_Position = gl_in[0].gl_Position+vec4(0.1, -0.1, 0.0, 0.0); // 右下角
	EmitVertex();

	gl_Position = gl_in[0].gl_Position+vec4(-0.1, 0.1, 0.0, 0.0); // 左上角
	EmitVertex();

 	gl_Position = gl_in[0].gl_Position+vec4(0.1, 0.1, 0.0, 0.0); // 右上角
	EmitVertex();

	gl_Position = gl_in[0].gl_Position+vec4(0.0, 0.2, 0.0, 0.0); // 顶
	fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();

	EndPrimitive();

}