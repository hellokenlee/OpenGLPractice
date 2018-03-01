#version 330 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vTanget;
layout (location = 3) in vec3 vBitanget;
layout (location = 4) in vec3 vNormal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform Light vLight;

out vec2 fTexCoord;
out vec3 fPosition;
out mat3 fTBN;
out Light fLight;

void main(){
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	
	fTexCoord = vTexCoord;
	fPosition = vec3(model * vec4(vPosition, 1.0));

	fLight.position = vLight.position;
	fLight.ambient = vec3(0.00);
	fLight.diffuse = vec3(0.0);
	fLight.specular = vec3(1.0);

	vec3 T = normalize(vec3(model * vec4(vTanget, 0.0)));
	vec3 B = normalize(vec3(model * vec4(vBitanget, 0.0)));
	vec3 N = normalize(vec3(model * vec4(vNormal, 0.0)));
	fTBN = mat3(T, B, N);

}