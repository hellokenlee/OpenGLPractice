#version 330 core

in vec2 fTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

out vec4 color;

void main(){
	color = vec4(texture(gNormal, fTexCoord).rgb, 1.0);
}