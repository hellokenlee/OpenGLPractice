#version 330 core

in vec2 fTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

uniform mat4 projection;

out float color;

// 重新调整ssaoNoise的纹理坐标
const vec2 noiseScale = vec2(800.0 / 4.0, 600.0 / 4.0);
//const vec2 noiseScale = vec2(1, 1);
const int kernelSize = 64;	// 总采样数
const float radius = 0.5;	// 采样半球域的半径
const float bias = 0.025;	// 微调周围的环境深度，防止过暗

void main(){
	// 从纹理中获取数值
	vec3 fragPos = texture(gPosition, fTexCoord).xyz;
	vec3 normal = normalize(texture(gNormal, fTexCoord).xyz);
	vec3 randomVec = texture(texNoise, fTexCoord * noiseScale).xyz;
	// 构造TBN矩阵,
	// 施密特正交化(因为normal原本就是单位化了，所以不用/dot(normal, normal))
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	// 统计kernel中有多少个sample的位置是被遮挡的
	float occlusion = 0.0;
	// 计算SSAO
	for(int i = 0; i < kernelSize; ++i){
		// 把样本从切线空间切换到视角空间
		vec3 sample = TBN * samples[i];
		sample = fragPos + (sample * radius);
		// 把样本坐标转换到屏幕空间，因为我们只有屏幕空间的数据
		vec4 offset = vec4(sample, 1.0);
		offset = projection * offset; // 执行投影操作
		offset.xyz /= offset.w;		  // 透视除法
		offset.xyz = offset.xyz * 0.5 + 0.5;	// 单位化到[0.0, 1.0]
		// 获取样本对应NDC位置的深度信息
		float sampleDepth = texture(gPosition, offset.xy).z;
		// 叠加
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));	// 柔化太远的像素，是的越远贡献越低
		occlusion += (sampleDepth >= (sample.z + bias) ? 1.0 : 0.0) * rangeCheck;
	}
	color = 1.0 - (occlusion / kernelSize);
}