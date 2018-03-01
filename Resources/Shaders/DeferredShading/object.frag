#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT{
	vec2 texCoord;
	vec3 position;
	vec3 normal;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
	// 均为世界坐标系下
	// 位置
	gPosition = fs_in.position;
	// 法向量
	gNormal = fs_in.normal;
	// 漫反射
	gAlbedoSpec.rgb = texture(texture_diffuse1, fs_in.texCoord).rgb;
	// 镜面反射
	gAlbedoSpec.a = texture(texture_specular1, fs_in.texCoord).r;
}