#version 330 core
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform Light vLight;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out Light light;
out vec3 vertexNormal;
out vec3 fragPos;
out vec2 fragTexCoords;


void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    fragTexCoords = texCoords;
    //计算顶点在View Space的坐标
    fragPos=vec3(view*model*vec4(position,1.0));
    //计算法线在View Space的坐标
    vertexNormal=mat3(view*model)*normal;
    //计算光源在View Space的坐标
    light.position=vec3(view*vec4(vLight.position,1.0));
    light.ambient=vLight.ambient;
    light.diffuse=vLight.diffuse;
    light.specular=vLight.specular;
}