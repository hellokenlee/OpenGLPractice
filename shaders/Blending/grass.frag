#version 330 core

in vec2 fTexcoord;

uniform sampler2D texture1;

out vec4 color;

void main()
{             
    
    vec4 texColor = texture(texture1, fTexcoord);
    if(texColor.a < 0.2)
    	discard;
    color=texColor;
}