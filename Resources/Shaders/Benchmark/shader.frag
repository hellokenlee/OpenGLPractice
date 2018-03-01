#version 330 core

in vec2 vertexTexCoord;
in vec3 vertexColor;

out vec4 color;

uniform sampler2D tex;//texture var

void main(){
	color=texture(tex, vertexTexCoord)*vec4(vertexColor,1.0f);
	//color = vec4(vertexColor,1.0f);
	//color = mix(texture(tex1,vertexTexCoord),texture(tex2,vertexTexCoord),0.8f);
}