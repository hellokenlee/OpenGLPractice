#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out VS_OUT{
	vec3 fragPosition;
	vec3 normal;
	vec2 texCoord;
	vec4 fragPositionLightSpace;
} vs_out;

//
//	因为ShadowMap在以光源为视角的空间中，因此要把顶点转换到光源的空间才能判断是否阴影
//
void main(){
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    vs_out.fragPosition = vec3(model * vec4(vPosition, 1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * vNormal;
    vs_out.texCoord = vTexCoord;
    vs_out.fragPositionLightSpace = lightSpaceMatrix * vec4(vs_out.fragPosition, 1.0);
}