#version 400 core

// 定义输入数据的格式
layout(triangles, equal_spacing, cw) in;

// V和P矩阵
uniform mat4 view;
uniform mat4 projection;

// 输入结构体
struct OutputPatch{
	vec3 position_B030;
	vec3 position_B021;
	vec3 position_B012;

	vec3 position_B003;
	vec3 position_B102;
	vec3 position_B201;

	vec3 position_B300;
	vec3 position_B210;
	vec3 position_B120;

	vec3 position_B111;

	vec3 normal[3];
};
// 声明输入
in patch OutputPatch patch_TES_in;
// 声明输出
out vec3 position_FS_in;
out vec3 normal_FS_in;


// 插值函数
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
   	// gl_TessCoord 就是PG生成的顶点在原来三角形的质心坐标
   	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

// 贝塞尔三角形插值函数:
// b(u, v, w) = \sum_{i + j + k = 3}B_{ijk}\frac{3!}{i!j!k!}u^iv^jw^k
//            = B_{300}w^3 + B_{030}u^3 + B_{003}v^3 
//            + B_{210}3w^2u + B_{120}3wu^2 + B_{201}3w^2v + B_{021}3u^2v + B_{102}3wv^2 + B_{012}3uv^2
//            + B_{111}6wuv
void main() {
	// 先插值法向量
	normal_FS_in = interpolate3D(patch_TES_in.normal[0], patch_TES_in.normal[1], patch_TES_in.normal[2]);
	//
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float w = gl_TessCoord.z;
	//
	float uPow2 = u * u;
	float uPow3 = uPow2 * u;
	float vPow2 = v * v;
	float vPow3 = vPow2 * v;
	float wPow2 = w * w;
	float wPow3 = wPow2 * w;
	//
	position_FS_in = (patch_TES_in.position_B300 * wPow3) 
					+ (patch_TES_in.position_B030 * uPow3)
					+ (patch_TES_in.position_B003 * vPow3)
					+ (patch_TES_in.position_B210 * 3.0 * wPow2 * u)
					+ (patch_TES_in.position_B120 * 3.0 * uPow2 * w)
					+ (patch_TES_in.position_B201 * 3.0 * wPow2 * v)
					+ (patch_TES_in.position_B021 * 3.0 * uPow2 * v)
					+ (patch_TES_in.position_B102 * 3.0 * vPow2 * w)
					+ (patch_TES_in.position_B012 * 3.0 * vPow2 * u)
					+ (patch_TES_in.position_B111 * 6.0 * w * u * v);
	//
	gl_Position = projection * view * vec4(position_FS_in, 1.0);
}