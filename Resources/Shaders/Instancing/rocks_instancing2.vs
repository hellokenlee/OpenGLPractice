#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 instancedModel;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * instancedModel * vec4(position, 1.0f);
    TexCoords = texCoords;
}