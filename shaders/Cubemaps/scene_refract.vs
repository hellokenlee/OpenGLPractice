#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fPosition;
out vec3 fNormal;


void main()
{
    //位置
    gl_Position = projection * view * model * vec4(position, 1.0f);
    //顶点法线
    fNormal = mat3(transpose(inverse(model))) * normal;
    //顶点位置
    fPosition = vec3(model * vec4(position, 1.0f));
}