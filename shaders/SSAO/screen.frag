#version 330 core

struct Light{
	vec3 position;
	vec3 color;
};


in vec2 fTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;

uniform Light light;

out vec4 color;

void main(){
	// 从G-Buffer中获取所有像素的相关信息
	// 注意，这次是在view space
	vec3 fragPos = texture(gPosition, fTexCoord).rgb;
	vec3 normal = texture(gNormal, fTexCoord).rgb;
	vec3 albedo = texture(gAlbedoSpec, fTexCoord).rgb;
	float Specular = texture(gAlbedoSpec, fTexCoord).a;
	float ao = texture(ssao, fTexCoord).r;
	// 环境光照，应用AO
	vec3 _color = vec3(0.0);
	vec3 ambient = vec3(0.3 * albedo * ao);
	vec3 viewDir = normalize(-fragPos);
	// 漫反射
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * light.color;
	// 镜面反射
	vec3 halfway = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfway), 0.0), 8.0);
	vec3 speular = light.color * spec * vec3(Specular);
	// 衰减
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (1.0 + (0.09 * dist) + (0.0032 * dist * dist));
	_color = ambient + attenuation * (diffuse + speular);
	color = vec4(_color, 1.0);
	// _color = vec3(texture(ssao, fTexCoord).r);
	// color = vec4(_color, 1.0);
}