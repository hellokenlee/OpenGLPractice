#version 330 core

in vec4 fPosition;// 世界坐标系中

uniform vec3 lightPosition;
uniform float far_plane;

// 因为没有颜色缓冲，因此片段着色器为空；但深度缓冲会被刷新
void main(){
	// 计算到光源之间的距离
	float lightDistance = length(fPosition.xyz - lightPosition);
	// 根据透视距离单位化到[0,1]
	lightDistance = lightDistance / far_plane;
	// 写入深度
	gl_FragDepth = lightDistance;
}