#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fNormal;
out vec2 fTexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
  	fTexCoord = vTexCoord;
  	fNormal = vNormal;
}