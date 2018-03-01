#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT{
	vec3 position;
	vec3 normal;
} fs_in;

uniform vec3 lightPositions[10];
uniform vec3 lightColors[10];
uniform vec3 fViewPosition;
uniform int lightNum;


// 计算光照
vec3 calcLighting(vec3 lightPosition, vec3 lightColor){
	// 计算观察向量
	vec3 viewDir = normalize(fViewPosition - fs_in.position);
	// 环境光照
	vec3 ambient = vec3(0.0);
	// 漫反射
	vec3 norm = normalize(fs_in.normal);
	vec3 lightDir = normalize(lightPosition - fs_in.position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	// 镜面反射
	float spec = 0.0;
	// Blinn-Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
	vec3 specular =  spec * lightColor;
	// 计算衰减
	float distance = length(lightPosition - fs_in.position);
	float attenuation = 1.0 / (distance * distance);
	return attenuation * (ambient + diffuse + specular);
}

void main(){
	// 设置颜色为白色
	vec3 originColor = vec3(1.0);
	// 光照
	vec3 lighting = vec3(0.0);
	for(int i = 0; i < lightNum; ++i){
		lighting += calcLighting(lightPositions[i], lightColors[i]);
	}
	// 
	vec4 color = vec4(lighting * originColor, 1.0);
	FragColor = color;
	float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.5)
		BrightColor = color;
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}