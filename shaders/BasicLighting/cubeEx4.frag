#version 330 core

in float lightStrength;

out vec4 color;

uniform vec3 objColor;
uniform vec3 lightColor;

void main(){
	color = vec4(lightStrength*lightColor*objColor,1.0f);
}