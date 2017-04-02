#version 330 core
in vec3 fPosition;
in vec3 fNormal;

uniform samplerCube texture1;
uniform vec3 camPosition;

out vec4 color;

void main()
{             
	// 在world space 中计算
    // 入射向量
    vec3 I = normalize(fPosition - camPosition);
    // 出射向量
    vec3 R = reflect(I, normalize(fNormal));
    // 立方体贴图
    color = texture(texture1, R);
}