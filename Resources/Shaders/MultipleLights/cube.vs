#version 330 core
//nVidia GT650m 不支持for循环 强制展开for.
#pragma optionNV unroll all
//平行光
struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
//点光源
struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//衰减常数
	float constant;
	float linear;
	float quadratic;
};
//#define POINT_LIGHTS_NUMBER 4;
//输入
layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vTexcoord;
//输出
out vec3 fNormal;
out vec3 fPosition;
out vec2 fTexCoord;
out DirectionalLight fDirLight;
out PointLight fPointLights[4];
//Uniforms
uniform DirectionalLight vDirLight;
uniform PointLight vPointLights[4];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	//计算其他量
	gl_Position = projection * view * model * vec4(vPosition,1.0);
	fNormal=mat3(view * model) * vNormal;
	fPosition=vec3(view * model * vec4(vPosition,1.0));
	fTexCoord=vTexcoord;
	//传递平行光
	fDirLight.direction = mat3(transpose(inverse(view)))*vDirLight.direction;
	fDirLight.ambient = vDirLight.ambient;
	fDirLight.diffuse = vDirLight.diffuse;
	fDirLight.specular = vDirLight.specular;
	//传递点光源
	for(int i = 0; i < 4; ++i){
		fPointLights[i].position = vec3(view * vec4(vPointLights[i].position,1.0f));
		fPointLights[i].ambient = vPointLights[i].ambient;
		fPointLights[i].diffuse = vPointLights[i].diffuse;
		fPointLights[i].specular = vPointLights[i].specular;
		fPointLights[i].constant = vPointLights[i].constant;
		fPointLights[i].linear = vPointLights[i].linear;
		fPointLights[i].quadratic = vPointLights[i].quadratic;
	}
	

}