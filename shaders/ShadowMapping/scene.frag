#version 330 core
in vec2 fTexCoord;

out vec4 color;

uniform sampler2D texture1;

void main()
{             
    // color = texture(texture1, fTexCoord);
    color = vec4(0.0, 1.0, 1.0, 1.0);
}