#version 330 core

struct Light
{
	vec3 position;//tangent space
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 fTexCoord;
in vec3 fPosition;//tangent space
in vec3 fViewPosition;//tangent space
in Light fLight;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform bool normalMap;

out vec4 color;

void main(){
	// 法线贴图获取法线
	vec3 fNormal = vec3(0.0, 0.0, 1.0);
	if(normalMap){
		fNormal = (texture(texture_normal1, fTexCoord).rgb) * 2.0 - 1.0;
	}
	// 环境光照
	vec3 ambientColor = fLight.ambient * vec3(texture(texture_diffuse1, fTexCoord));
	// 漫反射
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(fLight.position - fPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseColor = fLight.diffuse * diff * vec3(texture(texture_diffuse1, fTexCoord));
	// 镜面反射
	vec3 viewDir = normalize(fViewPosition - fPosition);
	float spec = 0.0;
	// Blinn-Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(norm, halfwayDir), 0.0), 8.0);
	vec3 specularColor = fLight.specular * spec * vec3(texture(texture_diffuse1, fTexCoord));
	// 计算衰减
	float distance = length(fLight.position - fPosition);
	float attenuation = 1.0 / distance;
	// 片段颜色
	color = vec4(attenuation * (ambientColor + diffuseColor + specularColor), 1.0);
	// Gamma矫正
	color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
}

