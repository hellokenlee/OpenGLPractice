#version 330 core
//光照
struct Light
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
//材质属性
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


in vec2 texCoords;
in vec3 vertexNormal;
in vec3 fragPos;

out vec4 color;

uniform Light light;
uniform Material material;

void main(){
	vec3 lightDir = normalize(-fragPos);
	//环境光照
	vec3 ambient=light.ambient*vec3(texture(material.diffuse,texCoords));
	//正规法线
	vec3 norm = normalize(vertexNormal);
	//计算漫反射
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,texCoords));
	//计算镜面反射
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir= reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0),material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,texCoords));
	//计算衰减值
	float distance=length(-fragPos);
	float attenuation=1.0f / (light.constant + light.linear*distance +light.quadratic*(distance*distance));
	//计算内半径到外半径的平滑过度
	float theta = dot(lightDir, vec3(0.0f,0.0f,1.0f));
	float epsilon = (light.innerCutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	//返回片元颜色
	color = vec4(attenuation*(ambient+intensity*(diffuse+specular)),1.0f);
}