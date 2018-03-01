#version 330 core

struct Light
{
	vec3 position;//tangent space
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
uniform vec3 viewPosition;
uniform Light vLight;

out vec2 fTexCoord;
out vec3 fPosition;//tangent space
out vec3 fViewPosition;//tangent space
out Light fLight;

void main(){
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * vTanget);
	vec3 B = normalize(normalMatrix * vBitanget);
	vec3 N = normalize(normalMatrix * vNormal);    
	mat3 TBNInversed = transpose(mat3(T, B, N)); // world space -> tangent space

	fTexCoord = vTexCoord;
	fPosition = TBNInversed * vec3(model * vec4(vPosition, 1.0)); // local -> world -> tangent

	fLight.ambient = vec3(0.3);
	fLight.diffuse = vec3(0.5);
	fLight.specular = vec3(0.5);
	fLight.position = TBNInversed * vLight.position;//(already in world) world -> tangent

	fViewPosition = TBNInversed * viewPosition;//(already in world) world -> tangent

}