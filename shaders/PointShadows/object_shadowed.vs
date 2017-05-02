#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool invertNormal;

out VS_OUT{
	vec3 fragPosition;
	vec3 normal;
	vec2 texCoord;
} vs_out;

//
//	这次的ShadowMap是在世界坐标系中，因此不需要转换到LightSpace
//
void main(){
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    vs_out.fragPosition = vec3(model * vec4(vPosition, 1.0));
    vec3 normal = vNormal;
    if(invertNormal)
    	normal = -normal;
    vs_out.normal = transpose(inverse(mat3(model))) * normal;
    vs_out.texCoord = vTexCoord;
}