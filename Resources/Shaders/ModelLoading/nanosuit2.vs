#version 330 core

#define NR_POINT_LIGHTS 3   //点光源数量

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //衰减常数
    float constant;
    float linear;
    float quadratic;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform Light vLight[NR_POINT_LIGHTS];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out Light light[NR_POINT_LIGHTS];
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
    for(int i=0;i<NR_POINT_LIGHTS;++i){
        light[i].position=vec3(view*vec4(vLight[i].position,1.0));
        light[i].ambient=vLight[i].ambient;
        light[i].diffuse=vLight[i].diffuse;
        light[i].specular=vLight[i].specular;

        light[i].constant=vLight[i].constant;
        light[i].linear=vLight[i].linear;
        light[i].quadratic=vLight[i].quadratic;
    }
}