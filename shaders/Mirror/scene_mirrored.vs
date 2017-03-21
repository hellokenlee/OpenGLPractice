#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
out float dist2mirror;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 mirror_position;
uniform vec3 mirror_normal;

void main()
{
	//单位化法向量
	vec3 mirror_normal_norm=normalize(mirror_normal);
	//求出该点和镜子平面的距离
    float dist=dot((position - mirror_position), mirror_normal_norm);
    //算出对称点坐标
    vec3 mirrored_position=position + 2*dist*(-mirror_normal_norm);
    //

    gl_Position = projection * view * model * vec4(mirrored_position, 1.0f);

    TexCoords = texCoords;

    dist2mirror=dist;
}