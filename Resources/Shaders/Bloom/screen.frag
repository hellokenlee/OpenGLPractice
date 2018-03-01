#version 330 core

in vec2 fTexCoord;

uniform sampler2D image;
uniform sampler2D image_blur;
uniform float exposure;
uniform bool bloom;

out vec4 color;

void main(){
	const float gamma = 2.2;
	vec3 hdrColor = texture(image, fTexCoord).rgb;
	vec3 bloomColor = texture(image_blur, fTexCoord).rgb;
	// 相加
	if(bloom)
		hdrColor += bloomColor;
	// 色调映射
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	// gamma矫正
	result = pow(result, vec3(1.0 / gamma));
	color = vec4(result, 1.0);
}