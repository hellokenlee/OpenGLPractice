/*
	点光源着色器-顶点着色器
	输入
		vec3 P(Position);
		vec3 N(Normal);
		vec3 C(Color);
		uniform PL[](PointLights);
		uniform PL_NUM;		
*/
#version 330 core

#define LIGHTS_NUM 5

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform Light vLights[LIGHTS_NUM];

out VS_OUT{
	vec3 normal;
	vec3 color;
	vec3 position;
	Light lights[LIGHTS_NUM];
}vs_out;

void main(){
	gl_Position = projection * view * model * vec4(position,1.0);
	vs_out.normal = transpose(inverse(mat3(view * model))) * vNormal;
	vs_out.position = vec3(view * model vec4(vPosition, 1.0));
	vs_out.color = vColor;
	for(int i = 0; i < LIGHTS_NUM; ++i){
		vs_out.lights[i].position = vec3(view * model vec4(vLights[i].position, 1.0));
		vs_out.lights[i].ambient = vLights[i].ambient;
		vs_out.lights[i].diffuse = vLights[i].diffuse;
		vs_out.lights[i].specular = vLights[i].specular;	
	}
}