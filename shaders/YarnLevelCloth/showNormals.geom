#version 330 core
layout (lines) in;
layout (line_strip, max_vertices = 6) out;

in vec3 normal_GS_in[];

const float MAGNITUDE = 0.2;

void GenerateLine(int index){
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(MAGNITUDE * normal_GS_in[index], 0.0);
	EmitVertex();

	EndPrimitive();
}

void main(){
	GenerateLine(0);
	GenerateLine(1);
}