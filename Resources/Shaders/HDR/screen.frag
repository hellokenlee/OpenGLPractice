#version 330 core

in vec2 fTexCoord;

uniform sampler2D texture1;
uniform bool toLDR;
uniform float exposure;

out vec4 color;

void main(){

	vec3 hdrColor = texture(texture1, fTexCoord).rgb;
	// Reinhard tone mapping
	vec3 mapped = hdrColor;
	if(toLDR)
		mapped = vec3(1.0) - exp(-hdrColor * exposure);
	// Gamma 矫正
	mapped = pow(mapped, vec3(1.0 / 2.2));
	// 返回
	color = vec4(mapped, 1.0);
}

