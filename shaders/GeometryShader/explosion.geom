#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 texcoord;
	vec3 normal;
} gs_in[];

out vec2 fTexCoord;

uniform float time;

vec4 explode(vec4 postion, vec3 direction){
	float magnitude = 5.0;
	vec3 offset = direction * ((sin(time) + 1.0) / 2.0) * magnitude;
	return postion + vec4(offset, 0.0);
}

void main(){
	gl_Position = explode(gl_in[0].gl_Position, gs_in[0].normal);
	fTexCoord = gs_in[0].texcoord;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, gs_in[0].normal);
	fTexCoord = gs_in[1].texcoord;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, gs_in[0].normal);
	fTexCoord = gs_in[2].texcoord;
	EmitVertex();

	EndPrimitive();
}