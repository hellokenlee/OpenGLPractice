#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 worldPos;
	vec3 worldNormal;
	//vec3 texCoord;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(position, 1.0);

	vs_out.worldPos = vec3(model * vec4(position, 1.0));
	vs_out.worldNormal = transpose(inverse(mat3(model))) * position;
	//vs_out.texCoord = texCoord;
}