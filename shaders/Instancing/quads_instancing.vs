#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

out vec3 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (std140) uniform UBO{
	vec3 offsets[1000];
};

void main()
{
	gl_Position = vec4(vPosition + offsets[gl_InstanceID], 1.0);
	fColor = vColor;
}