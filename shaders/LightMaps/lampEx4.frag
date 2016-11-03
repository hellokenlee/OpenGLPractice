#version 330 core

out vec4 color;

in vec2 fTexCoord;

uniform sampler2D fEmission;
uniform vec3 lampColor;

void main(){
	vec3 emission = vec3(texture(fEmission,fTexCoord));
	color = vec4(emission.r,emission.g+0.35,emission.b,1.0f);

}