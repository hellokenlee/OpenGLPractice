#version 330 core

#define LIGHTS_NUM 128

struct Light{
	vec3 position;
	vec3 color;
};

in VS_OUT{
	vec2 texCoord;
	vec3 position;
	vec3 normal;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform Light lights[LIGHTS_NUM];
uniform vec3 viewPos;

out vec4 color;

void main(){
	// 初始化
	vec3 fragPos = fs_in.position;
	vec3 fragNormal = normalize(fs_in.normal);
	vec3 fragAlbedo = texture(texture_diffuse1, fs_in.texCoord).rgb;
	float Specular = texture(texture_specular1, fs_in.texCoord).r;
	//计算光照
	vec3 _color = fragAlbedo * 0.1; // 环境光照
	vec3 viewDir = normalize(viewPos - fragPos);
	for(int i = 0; i < LIGHTS_NUM; ++i){
		vec3 lightDir = normalize(lights[i].position - fragPos);
		// 漫反射
		vec3 diffuse = max(dot(fragNormal, lightDir), 0.0) * fragAlbedo * lights[i].color;
		// 镜面反射
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(fragNormal, halfwayDir), 0.0), 16.0);
		vec3 specular = vec3(Specular) * spec * lights[i].color;
		// 衰减
		float dist = length(lights[i].position - fragPos);
		float attenuation = 1.0 / (1.0 + (0.7 * dist) + (1.8 * dist * dist));
		_color += attenuation * (diffuse + specular);
	}
	color = vec4(_color, 1.0);
}