#version 400 core

layout(vertices = 4) out;

// Context: `glPatchParameteri(GL_PATCH_VERTICES, PATCH_SIZE);`
#define PATCH_SIZE 4

//PatchSize = PATCH_SIZE in context
in vec3 position_TCS_in[];

// TCS's Output
out vec3 position_TES_in[];

uniform float tessLevelOuter0;

void main() {
	//
	gl_TessLevelOuter[0] = tessLevelOuter0;
	gl_TessLevelInner[1] = 2.0;
	//
	position_TES_in[gl_InvocationID] = position_TCS_in[gl_InvocationID];
}
	