#version 330 core

// 材质定义
struct Material{
	vec3 albedo;
	float metallic;
	float roughness;
	float ao;
};

// 光缘定义
struct Light{
	vec3 worldPos;
	vec3 color;
};

// 输入数据
in VS_OUT{
	vec3 worldPos;
	vec3 worldNormal;
	vec3 texCoord;
} fs_in;
// Uniform
uniform Light lights[4];
uniform Material material;
uniform vec3 viewPos;
// 片元颜色
out vec4 color;
// π
const float PI = 3.14159265359;
//
//	正态分布函数D， Trowbridge-Reitz GGX :
//		latex公式: NDF_{GGX TR}(n, h, \alpha) = \frac{\alpha^2}{\pi((n \cdot h)^2 (\alpha^2 - 1) + 1)^2}
// 
float DistributionGGX(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = denom * denom * PI;

	return nom /  denom;
}

//	菲涅尔方程的Schlick近似
vec3 FresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// 
//	几何函数，估算光线被遮蔽的概率
//	
float GeometrySchlickGGX(float NdotV, float roughness){
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) +k;

	return nom / denom;
}

//
//	几何函数，史密斯法： ggx1为几何遮蔽量 / ggx2为几何阴影量
// 
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

void main(){
	vec3 N = normalize(fs_in.worldNormal);
	vec3 V = normalize(fs_in.viewPos - fs_in.worldPos);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, material.albedo, material.metallic);

	// 出射量
	vec3 Lo = vec3(0.0);
	// 迭代计算光照
	for(int i = 0; i < 4; ++i){
		// 预计算
		vec3 L = normalize(lights[i].worldPos - fs_in.worldPos);// 光照方向
		vec3 H = normalize(V + L);
		float distance = length(lights[i].worldPos - fs_in.worldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lights[i].color * attenuation;
		// 计算Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, material.roughness);
		float G = GeometrySmith(N, V, L, material.roughness);
		vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F:
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - material.metallic;

		vec3 nominator = NDF * G * F;//分子
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;//分母，  防止分母为0, 加上0.0001
		vec3 specular = nominator / denominator;

		// 叠加Lo
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}
	// 计算环境光
	vec3 ambient = vec3(0.03) * material.albedo * material.ao;
	vec3 _color = ambient + Lo;
	// HDR && gamma矫正
	_color = _color / (_color + vec3(1.0));
	_color = pow(_color, vec3(1.0 / 2.2));
	//
	color = vec4(_color, 1.0);
}