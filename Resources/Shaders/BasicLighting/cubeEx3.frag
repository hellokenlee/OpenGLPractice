#version 330 core

in vec3 vertexNormal;
in vec3 fragPos;
in vec3 lightPos;

out vec4 color;

uniform vec3 objColor;
uniform vec3 lightColor;

void main(){
	//color = vec4(lightColor*objColor,1.0f);
	//计算环境光照
	float ambientStrength=0.1f;
	vec3 ambient = ambientStrength*lightColor;
	//正规法线
	vec3 norm = normalize(vertexNormal);
	//计算漫反射
	vec3 lightDir = normalize(lightPos-fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	//计算镜面反射
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir= reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0),32);
	vec3 specular = specularStrength * spec * lightColor;
	color = vec4((ambient+diffuse+specular)*objColor,1.0f);
}