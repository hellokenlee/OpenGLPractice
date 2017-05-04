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

const float minLayers = 8;
const float maxLayers = 32;

// 完成陡峭视差贴图
vec2 parallaxMapping(vec2 texCoord, vec3 viewDir){
	// 根据视角向量定义迭代层数
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
	float layerDepth = 1.0 / numLayers;
	// 初始化
	float currentLayerDepth = 0.0;
	vec2 P = viewDir.xy * heightScale;
	vec2 deltaTexCoord = P / numLayers;
	vec2 currentTexCoord = texCoord;
	float currentDepthMapValue = texture(texture_depth1, currentTexCoord).r;
	// 迭代找到比较接近的
	while(currentLayerDepth < currentDepthMapValue){
		currentTexCoord -= deltaTexCoord;
		currentDepthMapValue = texture(texture_depth1, currentTexCoord).r;
		currentLayerDepth += layerDepth;
	}
	// 返回
	return currentTexCoord;
}

// 完成视差遮蔽贴图(对上一种方法的前后个P点插值)
vec2 parallaxMapping2(vec2 texCoord, vec3 viewDir){
	// 根据视角向量定义迭代层数
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
	float layerDepth = 1.0 / numLayers;
	// 初始化
	float currentLayerDepth = 0.0;
	vec2 P = viewDir.xy * heightScale;
	vec2 deltaTexCoord = P / numLayers;
	vec2 currentTexCoord = texCoord;
	float currentDepthMapValue = texture(texture_depth1, currentTexCoord).r;
	// 迭代找到比较接近的
	while(currentLayerDepth < currentDepthMapValue){
		currentTexCoord -= deltaTexCoord;
		currentDepthMapValue = texture(texture_depth1, currentTexCoord).r;
		currentLayerDepth += layerDepth;
	}
	// 反循环向前走一步
	vec2 prevTexCoord = currentTexCoord + deltaTexCoord;
	// 获取两个P的深度
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(texture_depth1, prevTexCoord).r - currentLayerDepth + layerDepth;
	// 插值
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoord = prevTexCoord * weight + currentTexCoord * (1.0 - weight);
	// 返回
	return finalTexCoord;
}
void main(){
	// 计算观察向量
	vec3 viewDir = normalize(fViewPosition - fPosition);
	// 计算视差贴图偏移后的纹理坐标
	vec2 pTexCoord = fTexCoord;
	if(doParallaxMapping)
		pTexCoord = parallaxMapping2(fTexCoord, viewDir);
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

