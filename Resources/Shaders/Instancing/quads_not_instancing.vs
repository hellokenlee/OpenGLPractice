#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

out vec3 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = projection * view * model * vec4(vPosition, 1.0);
	gl_Position = model * vec4(vPosition, 1.0);
	fColor = vColor;
}