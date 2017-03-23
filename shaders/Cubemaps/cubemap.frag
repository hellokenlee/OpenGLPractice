#version 330 core
in vec3 TexCoords;

out vec4 color;

uniform samplerCube cubeTexture;

void main()
{             
    color = texture(cubeTexture, TexCoords);
}