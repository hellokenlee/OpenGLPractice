#version 330 core

in vec2 fTexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

out vec4 color;

void main()
{    
    color = vec4(texture(texture_diffuse1, fTexCoord));
}

