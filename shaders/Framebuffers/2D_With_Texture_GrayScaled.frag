#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture1;

void main()
{             
	vec4 origin=texture(texture1, TexCoords);
	float grayScale=(origin.r+origin.g+origin.b)/3.0f;
	color=vec4(grayScale,grayScale,grayScale,1.0f);
}