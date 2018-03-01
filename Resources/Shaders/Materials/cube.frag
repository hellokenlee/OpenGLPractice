#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vertexNormal;
in vec3 fragPos;
in Light light;

out vec4 color;

uniform Material material;

void main(){
	//计算环境光照
	vec3 ambient = material.ambient*light.ambient;
	//正规法线
	vec3 norm = normalize(vertexNormal);
	//计算漫反射
	vec3 lightDir = normalize(light.position-fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;
	//计算镜面反射
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir= reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0),material.shininess);
	vec3 specular = material.specular * spec * light.specular;
	//返回片元颜色
	color = vec4((ambient+diffuse+specular),1.0f);
}