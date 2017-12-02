#version 330 core
// 防止显卡不支持`array[i]`访问
#pragma optionNV unroll all
// 光源数目， C++会修改这里
#define LIGHTS_NUM 5
// 光源结构体
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
// 模型输入
layout (location = 0) in vec3 position_VS_in;
layout (location = 1) in vec3 normal_VS_in;
layout (location = 2) in vec2 texCoord_VS_in;
layout (location = 3) in mat4 model_VS_in;
// VP矩阵
uniform mat4 view;
uniform mat4 projection;
// 光源参数数组
uniform Light lights_VS_in[LIGHTS_NUM];
//输出
out vec3 normal_FS_in;
out vec2 texCoord_FS_in;
out vec3 position_FS_in;
out Light lights_FS_in[LIGHTS_NUM];

void main() {
	// 写入顶点位置
    gl_Position = projection * view * model_VS_in * vec4(position_VS_in, 1.0);
    
    // 传给FS法向量
	normal_FS_in = transpose(inverse(mat3(view * model_VS_in))) * normal_VS_in;
	// 传给FS在视角空间的片元位置
	position_FS_in = vec3(view * model_VS_in * vec4(position_VS_in, 1.0));
	// 传给FS纹理坐标
	texCoord_FS_in = texCoord_VS_in;

	// 传给FS光源信息
	for(int i = 0; i < LIGHTS_NUM; ++i){
		// 把光源位置转换为视角位置
		lights_FS_in[i].position = vec3(view * vec4(lights_VS_in[i].position, 1.0));
		// 其他信息直接传
		lights_FS_in[i].ambient = lights_VS_in[i].ambient;
		lights_FS_in[i].diffuse = lights_VS_in[i].diffuse;
		lights_FS_in[i].specular = lights_VS_in[i].specular;	
	}
   
}