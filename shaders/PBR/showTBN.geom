#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT{
	vec3 tangent;
	vec3 bitangent;
	vec3 normal;
} gs_in[];

out vec3 fColor;

const float MAGNITUDE = 0.2;

void GenerateTBN(vec4 pos){
	// gl_Position = pos + vec4(0.05 * gs_in[0].tangent, 0.0);
	// EmitVertex();
	// gl_Position = pos + vec4(MAGNITUDE * gs_in[0].tangent, 0.0);
	// EmitVertex();
	// fColor = vec3(1.0, 0.0, 0.0);
	// EndPrimitive();

	// gl_Position = pos + vec4(0.05 * gs_in[0].tangent, 0.0);
	// EmitVertex();
	// gl_Position = pos + vec4(MAGNITUDE * gs_in[0].bitangent, 0.0);
	// EmitVertex();
	// fColor = vec3(0.0, 1.0, 0.0);
	// EndPrimitive();

	gl_Position = pos + vec4(0.05 * gs_in[0].tangent, 0.0);
	EmitVertex();
	gl_Position = pos + vec4(MAGNITUDE * gs_in[0].normal, 0.0);
	EmitVertex();
	fColor = vec3(0.0, 0.0, 1.0);
	EndPrimitive();
}
void main(){
	vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position ) / 3.0;
	GenerateTBN(center);		
}