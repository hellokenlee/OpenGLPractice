#version 330 core

#define FNR_POINT_LIGHTS 3   //点光源数量

struct Light
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

in Light light[FNR_POINT_LIGHTS];
in vec2 fragTexCoords;
in vec3 fragPos;
in vec3 vertexNormal;

uniform float shininess;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

out vec4 color;

void main(){ 
	//正规化法线
	vec3 norm=normalize(vertexNormal);
	//点光源累加结果
	vec3 res=vec3(0.0,0.0,0.0);
	for(int i=0;i<FNR_POINT_LIGHTS;++i){
		//环境光照
		vec3 ambient=light[i].ambient*vec3(texture(texture_diffuse1, fragTexCoords));
		//漫反射光照
		vec3 lightDir=normalize(light[i].position - fragPos);
		float diff=max(dot(norm,lightDir),0.0);
		vec3 diffuse=light[i].diffuse*diff*vec3(texture(texture_diffuse1, fragTexCoords));
		//镜面反射光照
		vec3 viewDir=normalize(-fragPos);
		vec3 reflectDir=reflect(-lightDir,norm);
		float spec=pow(max(dot(viewDir,reflectDir),0.0),shininess);
		vec3 specular=light[i].specular*spec*vec3(texture(texture_specular1,fragTexCoords));
		//衰减值
		float distance = length(light[i].position - fragPos);
		float attenuantion=1.0f/(light[i].constant + light[i].linear*distance + light[i].quadratic * (distance * distance));
		res+=(ambient+diffuse+specular)*attenuantion;
	}
	//写入颜色
	color=vec4(res, 1.0f);
}

