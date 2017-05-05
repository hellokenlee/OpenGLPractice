#version 330 core

in vec2 fTexCoord;

uniform sampler2D texture;
uniform bool toLDR;

out vec4 color;

const float exposure = 0.1;

void main(){

	vec3 hdrColor = texture(texture, fTexCoord).rgb;
	// Reinhard tone mapping
	vec3 mapped = hdrColor;
	if(toLDR)
		mapped = vec3(1.0) - exp(-hdrColor * exposure);
	// Gamma 矫正
	mapped = pow(mapped, vec3(1.0 / 2.2));
	// 返回
	color = vec4(mapped, 1.0);
}

