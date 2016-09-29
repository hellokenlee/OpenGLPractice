#version 330 core

in vec2 vertexTexCoord;
in vec3 vertexColor;

out vec4 color;

uniform sampler2D tex1;//texture var
uniform sampler2D tex2;

void main(){
	//color=vec4(vectexColor,1.0);
	//color = mix(texture(tex1,vertexTexCoord),texture(tex2,vertexTexCoord),0.8f);
	color = mix( texture(tex1, vertexTexCoord).rgba, texture(tex2, vec2(-vertexTexCoord.x,-vertexTexCoord.y)).rgba, vec4(texture(tex2, vec2(-vertexTexCoord.x,-vertexTexCoord.y))).a * 0.2);
}