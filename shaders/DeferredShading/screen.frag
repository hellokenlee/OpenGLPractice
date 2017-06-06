#version 330 core

#define LIGHTS_NUM 32

struct Light{
	vec3 position;
	vec3 color;
};

in vec2 fTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform Light lights[LIGHTS_NUM];
uniform vec3 viewPos;

out vec4 color;

void main(){
	// 从G-Buffer中获取所有像素的相关信息
	vec3 fragPos = texture(gPosition, fTexCoord).rgb;
	vec3 fragNormal = normalize(texture(gNormal, fTexCoord).rgb);
	vec3 fragAlbedo = texture(gAlbedoSpec, fTexCoord).rgb;
	float Specular = texture(gAlbedoSpec, fTexCoord).a;
	// 计算光照
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
	// float spec = texture(tex1, fTexCoord).a;
	// color = vec4(spec, spec, spec, 1.0);
	// //color = vec4(texture(tex1, fTexCoord).rgb, 1.0);
	// //color = vec4(1.0);
}