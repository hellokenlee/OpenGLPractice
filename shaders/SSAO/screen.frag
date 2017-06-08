#version 330 core

in vec2 fTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;

out vec4 color;

void main(){
	vec3 _color = vec3(texture(ssao, fTexCoord).r);
	color = vec4(_color, 1.0);
}