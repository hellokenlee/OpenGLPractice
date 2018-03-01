#version 330 core

layout (location = 0) in vec3 position_VS_in;
layout (location = 1) in vec3 normal_VS_in;

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform Light light;

out vec3 position_FS_in;
out vec3 normal_FS_in;
out Light light_FS_in;

void main(){
	// 写入顶点位置
	gl_Position = projection * view * model *vec4(position_VS_in, 1.0);
	// 向后传递片元位置，法向量
	position_FS_in = vec3(view * model * vec4(position_VS_in, 1.0));
	normal_FS_in = mat3(view * model) * normal_VS_in;
	// 向后传递光源信息
	light_FS_in.direction = mat3(transpose(inverse(view))) * light.direction;
	light_FS_in.ambient = vec3(0.2);
	light_FS_in.diffuse = vec3(0.6);
	light_FS_in.specular = vec3(0.5);	
}