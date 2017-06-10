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
const int kernelSize = 64;
const float radius = 0.5;
const float bias = 0.025;

void main(){
	// 从纹理中获取数值
	vec3 fragPos = texture(gPosition, fTexCoord).xyz;
	vec3 normal = normalize(texture(gNormal, fTexCoord).xyz);
	vec3 randomVec = texture(texNoise, fTexCoord * noiseScale).xyz;
	//vec3 randomVec = vec3(1.0, 1.0, 0.0);
	// 构造TBN矩阵,
	// 施密特正交化(因为normal原本就是单位化了，所以不用/dot(normal, normal))
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	// 计算SSAO
	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; ++i){
		// 把样本从切线空间切换到视角空间
		vec3 sample = TBN * samples[i];
		sample = fragPos + (sample * radius);
		//
		vec4 offset = vec4(sample, 1.0);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;
		// 获取样本的深度信息
		float sampleDepth = texture(gPosition, offset.xy).z;
		// 叠加
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= (sample.z + bias) ? 1.0 : 0.0) * rangeCheck;
	}
	occlusion = 1.0 - (occlusion / kernelSize);
	color = occlusion;
}