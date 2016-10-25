#version 330 core

in vec3 vertexNormal;
in vec3 fragPos;

out vec4 color;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main(){
	//color = vec4(lightColor*objColor,1.0f);
	float ambientStrength=0.1f;
	vec3 ambient = 0.1f*lightColor;
	vec3 norm = normalize(vertexNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	color = vec4((ambient+diffuse)*objColor,1.0f);
}