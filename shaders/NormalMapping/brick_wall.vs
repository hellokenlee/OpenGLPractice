#version 330 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform Light vLight;

out vec2 fTexCoord;
out vec3 fPosition;
out Light fLight;


void main(){
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	
	fTexCoord = vTexCoord;
	fPosition = vec3(model * vec4(vPosition, 1.0));

	fLight.position = vLight.position;
	fLight.ambient = vec3(0.05); //vLight.ambient;
	fLight.diffuse = vec3(0.5);
	fLight.specular = vec3(1.0);	
}