#version 330 core

in vec2 fTexCoord;

uniform sampler2D depthMap;

out vec4 color;

void main(){
	float depth = texture(depthMap, fTexCoord).r;
	color = vec4(vec3(depth), 1.0);
}

