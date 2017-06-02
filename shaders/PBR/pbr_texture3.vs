#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vTanget;
layout (location = 4) in vec3 vBitanget;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec2 texCoord;
	vec3 position;
	mat3 TBN;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	vec3 T = normalize(vec3(model * vec4(vTanget, 0.0)));
	vec3 B = normalize(vec3(model * vec4(vBitanget, 0.0)));
	vec3 N = normalize(vec3(model * vec4(vNormal, 0.0)));

	vs_out.TBN = mat3(T, B, N);
	vs_out.texCoord = vTexCoord;
	vs_out.position = vec3(model * vec4(vPosition, 1.0));
}