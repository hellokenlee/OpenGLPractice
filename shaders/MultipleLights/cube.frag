#version 330 core
//nVidia GT650m 不支持for循环 强制展开for.
#pragma optionNV unroll all
//材质属性
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
//平行光
struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
//点光源
struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//衰减常数
	float constant;
	float linear;
	float quadratic;
};
//光照
struct SpotLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //聚光灯半径范围(内半径，外半径)
    float innerCutOff;
    float outerCutOff;
    //衰减常数
    float constant;
    float linear;
    float quadratic;
};
//输入
in vec3 fNormal;
in vec3 fPosition;
in vec2 fTexCoord;
in DirectionalLight fDirLight;
in PointLight fPointLights[4];
//输出
out vec4 color;
//Uniforms
uniform Material fMaterial;
uniform SpotLight fSpotLight;

//计算平行光照的函数 声明
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
//计算点光源光照函数 声明
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);
//计算聚光灯函数 声明
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);
//主函数
void main(){
	//正规化
	vec3 normal=normalize(fNormal);
	vec3 viewDir=normalize(-fPosition);
	
	//平行光
	//vec3 fColor = vec3(0,0,0);
	vec3 fColor = calcDirLight(fDirLight, normal, viewDir);
	fColor = fColor + calcSpotLight(fSpotLight, normal, viewDir);

	//点光源
	for(int i=0;i<4;++i){
		fColor = fColor + calcPointLight(fPointLights[i], normal, viewDir);
	}

	/*
	fColor=fColor+calcPointLight(fPointLights[0], normal, viewDir);
	fColor=fColor+calcPointLight(fPointLights[1], normal, viewDir);
	fColor=fColor+calcPointLight(fPointLights[2], normal, viewDir);
	fColor=fColor+calcPointLight(fPointLights[3], normal, viewDir);
	*/	
	color = vec4(fColor, 1.0f);
}

//假设法线已经正规化了
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
	//正规化光照方向
	vec3 lightDir = normalize(-light.direction);
	//环境光照
	vec3 ambient = light.ambient * vec3(texture(fMaterial.diffuse, fTexCoord));
	//漫反射光照
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(fMaterial.diffuse, fTexCoord));
	//镜面反射光照
	vec3 reflectDir= reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), fMaterial.shininess);
	vec3 specular = light.specular * spec * vec3(texture(fMaterial.specular, fTexCoord));
	//叠加，返回
	return (ambient + diffuse + specular);
}

//假设法线已经正规化了
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir){
	//正规化光照方向
	vec3 lightDir = normalize(light.position - fPosition);
	//环境光照
	vec3 ambient = light.ambient * vec3(texture(fMaterial.diffuse, fTexCoord));
	//漫反射光照
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(fMaterial.diffuse, fTexCoord));
	//镜面反射
	vec3 reflectDir= reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), fMaterial.shininess);
	vec3 specular = light.specular * spec * vec3(texture(fMaterial.specular, fTexCoord));
	//计算衰减值
	float distance = length(light.position - fPosition);
	float attenuation = 1.0f / (light.constant + light.linear*distance + light.quadratic * (distance * distance));
	//叠加，乘衰弱，返回
	//return (ambient + diffuse + specular) * attenuation;
	return ((ambient+diffuse+specular)*attenuation);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-fPosition);
	//环境光照
	vec3 ambient=light.ambient*vec3(texture(fMaterial.diffuse,fTexCoord));
	//计算漫反射
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(fMaterial.diffuse, fTexCoord));
	//计算镜面反射
	vec3 reflectDir= reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0),fMaterial.shininess);
	vec3 specular = light.specular * spec * vec3(texture(fMaterial.specular,fTexCoord));
	//计算衰减值
	float distance=length(-fPosition);
	float attenuation=1.0f / (light.constant + light.linear*distance +light.quadratic*(distance*distance));
	//计算内半径到外半径的平滑过度
	float theta = dot(lightDir, vec3(0.0f,0.0f,1.0f));
	float epsilon = (light.innerCutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return (attenuation*(ambient+intensity*(diffuse+specular)));
}
