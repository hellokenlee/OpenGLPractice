#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

out float lightStrength;

uniform vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){

	gl_Position = projection * view * model * vec4(position,1.0f);
	//	--Gouraund Shading--//
	//计算在view space下的坐标
	vec3 vPos=vec3(view * model * vec4(position,1.0f));
	vec3 lightPos=vec3(view*vec4(LightPos,1.0f));
	//计算法线(view空间)
	vec3 norm=normalize(mat3(transpose(inverse(view*model))) * normal);
	//计算环境光照强度
	float ambient=0.1;
	//计算漫反射强度
	vec3 lightDir = normalize(lightPos - vPos);
	float diffuse = max(dot(norm, lightDir), 0.0);
	//计算镜面反射
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(-vPos);
	vec3 reflectDir= reflect(-lightDir,norm);
	float specular = specularStrength*pow(max(dot(viewDir,reflectDir), 0.0),32);
	
	lightStrength=ambient+diffuse+specular;
}