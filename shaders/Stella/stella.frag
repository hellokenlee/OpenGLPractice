#version 330 core

in vec2 fTexCoord;
in vec4 fColor;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{    
    color = fColor;
    //color = vec4(texture(texture_diffuse1, fTexCoord));
}

