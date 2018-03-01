#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 startPos;
uniform vec3 endPos;

out vec3 fColor;
void main()
{
    vec3 linesPos[2];
    linesPos[0]=startPos;
    linesPos[1]=endPos;
    gl_Position = projection * view * vec4(linesPos[gl_VertexID], 1.0f);
}