#version 330 core

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 position_FS_in;
in vec3 normal_FS_in;
in Light light_FS_in;

uniform vec3 fragmentColor;

out vec4 color;

void main(){
	// 环境光照
	vec3 ambientColor = light_FS_in.ambient * fragmentColor;
	// 单位化法向量
	vec3 normal = normalize(normal_FS_in);
	// 漫反射
	vec3 lightDir = normalize(-light_FS_in.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor = light_FS_in.diffuse * diff * fragmentColor;
	// 镜面反射
	vec3 viewDir = normalize(-position_FS_in);
	vec3 reflectDir= reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specularColor = light_FS_in.specular * spec * fragmentColor;
	// 片段颜色
	color = vec4((ambientColor + diffuseColor + specularColor), 1.0f);
}

