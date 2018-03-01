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
uniform sampler2D texture_depth1;
uniform float heightScale;
uniform bool doNormalMapping;
uniform bool doParallaxMapping;

out vec4 color;


// 完成视差贴图
vec2 parallaxMapping(vec2 texCoord, vec3 viewDir){
	// 从贴图获取深度
	float height = texture(texture_depth1, texCoord).r;
	// 计算偏移
	vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
	// 返回新的
	return texCoord - p;
}

void main(){
	// 计算观察向量
	vec3 viewDir = normalize(fViewPosition - fPosition);
	// 计算视差贴图偏移后的纹理坐标
	vec2 pTexCoord = fTexCoord;
	if(doParallaxMapping)
		pTexCoord = parallaxMapping(fTexCoord, viewDir);
	// 舍弃重复采样的边缘
	if(pTexCoord.x > 1.0 || pTexCoord.y > 1.0 || pTexCoord.x < 0.0 || pTexCoord.y < 0.0)
    	discard;
	// 法线贴图获取法线
	vec3 fNormal = vec3(0.0, 0.0, 1.0);
	if(doNormalMapping)
		fNormal = (texture(texture_normal1, pTexCoord).rgb) * 2.0 - 1.0;
	// 环境光照
	vec3 ambientColor = fLight.ambient * vec3(texture(texture_diffuse1, pTexCoord));
	// 漫反射
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(fLight.position - fPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseColor = fLight.diffuse * diff * vec3(texture(texture_diffuse1, pTexCoord));
	// 镜面反射
	float spec = 0.0;
	// Blinn-Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(norm, halfwayDir), 0.0), 8.0);
	vec3 specularColor = fLight.specular * spec * vec3(texture(texture_diffuse1, pTexCoord));
	// 计算衰减
	float distance = length(fLight.position - fPosition);
	float attenuation = 1.0 / distance;
	// 片段颜色
	color = vec4(attenuation * (ambientColor + diffuseColor + specularColor), 1.0);
	// Gamma矫正
	color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
}

