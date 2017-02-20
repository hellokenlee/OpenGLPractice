#version 330 core
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in Light light;
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
	//环境光照
	vec3 ambient=light.ambient*vec3(texture(texture_diffuse1, fragTexCoords));
	//漫反射光照
	vec3 lightDir=normalize(light.position - fragPos);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=light.diffuse*diff*vec3(texture(texture_diffuse1, fragTexCoords));
	//镜面反射光照
	vec3 viewDir=normalize(-fragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),shininess);
	vec3 specular=light.specular*spec*vec3(texture(texture_specular1,fragTexCoords));
	//写入颜色
	color=vec4((ambient+diffuse+specular),1.0f);
}

