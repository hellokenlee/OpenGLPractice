#version 330 core

in vec2 vertexTexCoord;

out vec4 color;

uniform sampler2D tex1;//texture var
uniform sampler2D tex2;

void main(){
	color = mix( texture(tex1, vertexTexCoord).rgba, texture(tex2, vertexTexCoord).rgba, vec4(texture(tex2, vertexTexCoord)).a * 0.5);
}