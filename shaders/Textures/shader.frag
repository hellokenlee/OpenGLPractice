#version 330 core

in vec2 vertexTexCoord;
in vec3 vertexColor;

out vec4 color;

uniform sampler2D tex;//texture var

void main(){
	//color=vec4(vectexColor,1.0);
	color = texture(tex,vertexTexCoord);
}