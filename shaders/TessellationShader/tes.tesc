#version 400 core

layout(vertices = 3) out;

in vec3 vPosition[];
out vec3 tcPosition[];

void main() {
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
}