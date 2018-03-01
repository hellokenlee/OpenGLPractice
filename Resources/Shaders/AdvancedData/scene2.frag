#version 330 core
in vec2 fTexCoord;
in vec3 fNormal;

uniform sampler2D texture1;

out vec4 color;

void main()
{             
    vec4 origin_color = texture(texture1, fTexCoord);
    vec4 normal_color = vec4((fNormal+1.0f)/2.0f,1.0f);
    color = (origin_color*0.8f) + (normal_color*0.2f);

}