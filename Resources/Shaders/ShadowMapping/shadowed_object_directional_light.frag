#version 330 core

in VS_OUT{
	vec3 fragPosition;
	vec3 normal;
	vec2 texCoord;
	vec4 fragPositionLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

out vec4 color;


float shadowCalculation(vec4 fragPositionLightSpace, vec3 normal, vec3 lightDir){

	// 透视除法
	vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w;
	// 转换到[0,1]
	projCoords = projCoords * 0.5 + 0.5;
	// 不阴影ShadowMap之外的像素
	if(projCoords.z > 1.0)
        return 0.0;
	/* 偏移防止阴影失真
	float depth = texture(shadowMap, projCoords.xy).r;
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float _shadow = (projCoords.z) > depth ? 1.0f : 0.0f;
	return _shadow;
	//*/
	//* PCF 滤波: 核心思路，shadow不再是二元的0 or 1。 而是周围8邻域的均值滤波
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -2; x <= 2; ++x){
		for(int y = -2; y<= 2; ++y){
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += projCoords.z > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	return shadow;
	//*/
}

void main(){
	vec3 originColor = texture(diffuseTexture, fs_in.texCoord).rgb;
	// 单位化法线
	vec3 normal = normalize(fs_in.normal);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	// 环境光照
	vec3 ambient = 0.2 * originColor;//???
	// 漫反射
	vec3 lightDir = normalize(lightPosition - fs_in.fragPosition);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// 镜面反射
    vec3 viewDir = normalize(viewPosition - fs_in.fragPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // 计算阴影
    float shadow = shadowCalculation(fs_in.fragPositionLightSpace, normal, lightDir);
    // 返回最终颜色
    vec3 _color = (ambient + (1.0 - shadow) * (diffuse + specular)) * originColor;

    color = vec4(_color, 1.0);
}