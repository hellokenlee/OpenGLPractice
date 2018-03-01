#version 330 core
layout (location = 0) in vec3 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 normal;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
    
    vs_out.normal = mat3(projection * view * model) * vPosition;
}