#version 330 core

in vec2 vertexTexCoord;
in vec3 vertexColor;

out vec4 color;

uniform sampler2D tex1;//texture var
uniform sampler2D tex2;

void main(){
	color = mix( texture(tex1, vertexTexCoord*2).rgba, texture(tex2, vertexTexCoord*2).rgba, vec4(texture(tex2, vertexTexCoord*2)).a * 0.5);
}