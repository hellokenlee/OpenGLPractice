#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float inverseNormal;

out VS_OUT{
	vec2 texCoord;
	vec3 position;
	vec3 normal;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(position,1.0);

	vs_out.texCoord = texCoord;
	vs_out.position = vec3(model * vec4(position, 1.0));
	vs_out.normal = inverseNormal * transpose(inverse(mat3(model))) * normal;
}