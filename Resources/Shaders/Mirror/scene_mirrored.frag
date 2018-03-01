#version 330 core
in vec2 TexCoords;
in float dist2mirror;

out vec4 color;

uniform sampler2D texture1;

void main()
{             
//     vec4 origin = texture(texture1, TexCoords);
//     float aver=(origin.r+origin.g+origin.b)/3.0;
//     color=vec4(aver,aver,aver,1.0);
		if(dist2mirror<=0){
			discard;
		}
		color = texture(texture1, TexCoords);
}