#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in mat4 vModel;
layout (location = 6) in vec4 vColor;

out vec2 fTexCoord;
out vec4 fColor;

uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * vModel *vec4(vPosition, 1.0);
    fTexCoord = vTexCoord;
    fColor = vColor;
}