#version 330 core
// 光源数目， C++会修改这里
#define LIGHTS_NUM 0
// 光源结构体
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
// 输入
in vec3 normal_FS_in;
in vec3 color_FS_in;
in vec3 position_FS_in;
in Light lights_FS_in[LIGHTS_NUM];
// 输出
out vec4 color_FS_out;


void main(){
	// 在视角空间着色
	vec3 normal = normalize(normal_FS_in);
	// 原始颜色
	vec3 _color = vec3(0.0);
	// 逐个叠加着色信息
	for(int i = 0; i < LIGHTS_NUM; ++i){
		// 环境光
		vec3 ambient += lights_FS_in[i].ambient;
		// 入射光方向
		vec3 lightDir = normalize(lights_FS_in[i].position);
		// 漫反射
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse += flights_FS_in[i].diffuse * diff;
		// 视角方向
		vec3 viewDir = normalize(fs_in.position);
		// 半程向量
		vec3 halfway = normalize(lightDir);
		// 镜面反射
		float spec = pow(max(dot(normal, halfway), 0.0), 4.0);
		vec3 specular += lights_FS_in[i].specular * spec;
		// 离光源距离
		float distance = length(light.position);
		// 衰减
		float attenuation = 1.0f / (distance * distance);
		// 叠加着色
		_color += attenuation * (ambient + diffuse + specular);
	}
	// 输出
	color = vec4(_color, 1.0);
}