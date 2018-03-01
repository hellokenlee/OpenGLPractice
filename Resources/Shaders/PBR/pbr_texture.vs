#version 330 core

// 光源定义
struct Light{
	vec3 position;
	vec3 color;
};

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vTanget;
layout (location = 4) in vec3 vBitanget;
layout (location = 5) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform Light lights[4];

out VS_OUT{
	vec2 texCoord;
	vec3 posTanSpace;
	vec3 viewPosTanSpace;
	Light lights[4];
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	mat3 normalMatrix = mat3(model);//tricks 没有不同方向的形变
	//mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * vTanget);
	vec3 B = normalize(normalMatrix * vBitanget);
	vec3 N = normalize(normalMatrix * vNormal);    
	mat3 TBNInversed = transpose(mat3(T, B, N)); // world space -> tangent space

	vs_out.texCoord = vTexCoord;
	vs_out.posTanSpace = TBNInversed * vec3(model * vec4(vPosition, 1.0));// local -> world -> tangent
	vs_out.viewPosTanSpace = TBNInversed * viewPos;// world space -> tangent space

	for(int i = 0; i < 4; ++i){
		vs_out.lights[i].position = TBNInversed * lights[i].position;// world space -> tangent space
		vs_out.lights[i].color = lights[i].color;
	}
}