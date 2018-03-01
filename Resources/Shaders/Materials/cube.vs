#version 330 core
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

out vec3 vertexNormal;
out vec3 fragPos;
out Light light;

uniform Light vLight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
	gl_Position = projection * view * model * vec4(position,1.0f);
	fragPos = vec3(view * model * vec4(position,1.0f));
	//vertexNormal = mat3(transpose(inverse(view*model))) * normal;
	vertexNormal = mat3(view*model) * normal;
	light.position=vec3(view * vec4(vLight.position,1.0f));
	light.ambient=vLight.ambient;
	light.diffuse=vLight.diffuse;
	light.specular=vLight.specular;
	
}