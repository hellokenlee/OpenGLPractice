/*
	点光源着色器-片段着色器
	输入
		vec3 P(Position);
		vec3 N(Normal);
		vec3 C(Color);
		uniform PL[](PointLights);
		uniform PL_NUM;		
*/
#version 330 core

#define LIGHTS_NUM 5

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT{
	vec3 normal;
	vec3 color;
	Light lights[LIGHTS_NUM];
}fs_in;

out vec4 color;

void main(){
	// Do Shading in view-space
	vec3 N = normalize(fs_in.normal);
	vec3 _color = vec3(0.0);
	for(int i = 0; i < LIGHTS_NUM; ++i){
		vec3 ambient += fs_in.lights[i].ambient;

		vec3 lightDir = normalize(fs_in.lights[i].position);
		float diff = max(dot(N, lightDir), 0.0);
		vec3 diffuse += fs_in.lights[i].diffuse * diff;

		vec3 viewDir = normalize(fs_in.position);
		vec3 H = normalize(lightDir);
		float spec = pow(max(dot(N, H), 0.0), 4.0);
		vec3 specular += fs_in.lights[i].specular * spec;

		float distance = length(light.position);
		float attenuation = 1.0f / (distance * distance);

		_color += attenuation * (ambient + diffuse + specular);
	}

	color = vec4(_color, 1.0);
}