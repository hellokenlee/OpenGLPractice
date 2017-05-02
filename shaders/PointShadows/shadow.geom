#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 lightSpaceMatrices[6];

out vec4 fPosition;

void main(){
	for(int face = 0; face < 6; ++face){
		// 对于CubeMap的每一面
		gl_Layer = face;
		for(int i = 0; i < 3; ++i){
			// 对于三角形的每一个顶点
			fPosition = gl_in[i].gl_Position;
			gl_Position = lightSpaceMatrices[face] * fPosition;
			EmitVertex();
		}
		EndPrimitive();
	}
}