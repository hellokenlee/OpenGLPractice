#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vTanget;
layout (location = 4) in vec3 vBitanget;
layout (location = 5) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 tangent;
	vec3 bitangent;
	vec3 normal;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * vTanget);
	vec3 B = normalize(normalMatrix * vBitanget);
	vec3 N = normalize(normalMatrix * vNormal);    
	mat3 TBNInversed = transpose(mat3(T, B, N)); // world space -> tangent space

	vs_out.tangent = vTanget;
	vs_out.bitangent = vBitanget;
	vs_out.normal = vNormal;
}