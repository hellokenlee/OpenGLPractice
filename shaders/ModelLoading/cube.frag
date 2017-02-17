#version 330 core

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 texCoords;
in vec3 vertexNormal;
in vec3 fragPos;
in Light light;


out vec4 color;

uniform float shininess;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
	
	//环境光照
	vec3 ambient=light.ambient*vec3(texture(texture_diffuse1,texCoords));
	//正规法线
	vec3 norm = normalize(vertexNormal);
	//计算漫反射
	vec3 lightDir = normalize(light.position-fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1,texCoords));
	//计算镜面反射
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir= reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0),shininess);
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1,texCoords));
	//返回片元颜色
	color = vec4((ambient+diffuse+specular),1.0f);
	
	//color = vec4(1.0f,1.0f,1.0f,1.0f);
}