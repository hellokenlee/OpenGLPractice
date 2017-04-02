#version 330 core
in vec3 fPosition;
in vec3 fNormal;

uniform samplerCube texture1;
uniform vec3 camPosition;

out vec4 color;

void main()
{             
	// 透射比
	float ratio = 1.0f / 1.52f;
    // 入射向量
    vec3 I = normalize(fPosition - camPosition);
    // 出射向量
    vec3 R = refract(I, normalize(fNormal), ratio);
    // 立方体贴图
    color = texture(texture1, R);
}