#version 330 core

in VS_OUT{
	vec3 fragPosition;
	vec3 normal;
	vec2 texCoord;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube shadowMap;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform float far_plane;

out vec4 color;


float shadowCalculation(vec3 fragPosition){
	// 取点到光源的
	vec3 light2Frag = fragPosition - lightPosition;
	// 计算点到光源的深度
	float depth = texture(shadowMap, light2Frag).r;
	// 把深度还原为[0, far]
	depth *= far_plane;
	// 当前的深度
	float currentDepth =  length(light2Frag);
	// 计算shadow
	float bias = 0.05; 
	float shadow = currentDepth -  bias > depth ? 1.0 : 0.0;
	return shadow;
}

void main(){
	vec3 originColor = texture(diffuseTexture, fs_in.texCoord).rgb;
	// 单位化法线
	vec3 normal = normalize(fs_in.normal);
	vec3 lightColor = vec3(0.3);
	// 环境光照
	vec3 ambient = 0.3 * originColor;//???
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
    float shadow = shadowCalculation(fs_in.fragPosition);
    // 返回最终颜色
    vec3 _color = (ambient + (1.0 - shadow) * (diffuse + specular)) * originColor;
    color = vec4(_color, 1.0);
}