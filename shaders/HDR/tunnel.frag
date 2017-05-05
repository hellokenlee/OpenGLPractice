#version 330 core

in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fPosition;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 fViewPosition;

out vec4 color;

vec3 calcLighting(vec3 lightPosition, vec3 lightColor){
	// 计算观察向量
	vec3 viewDir = normalize(fViewPosition - fPosition);
	// 环境光照
	vec3 ambient = 0.001 * lightColor;
	// 漫反射
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(lightPosition - fPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	// 镜面反射
	float spec = 0.0;
	// Blinn-Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	// 计算衰减
	float distance = length(lightPosition - fPosition);
	float attenuation = 1.0 / (distance);
	return attenuation * (ambient + diffuse + specular);
}

void main(){
	vec3 originColor = vec3(texture(texture_diffuse1, fTexCoord));
	vec3 lighting = vec3(0.0);
	for(int i = 0; i < 4; ++i){
		lighting += calcLighting(lightPositions[i], lightColors[i]);
	}
	// 
	color = vec4(lighting * originColor, 1.0);
	// Gamma
	color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
}