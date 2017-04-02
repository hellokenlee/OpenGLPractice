#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

layout (std140) uniform CameraMatrix{
						//	base alignment		//	offset
	mat4 view;			//	64 in total			//	0 in total
						//	16					//	0 (row 1)
						//	16					//	16 (row 2)
						//	16					//	32 (row 3)
						//	16					//	48 (row 4)

	mat4 projection;	//	64 in total			//	64 in total
						//	16					//	64 (row 1)
						//	16					//	80 (row 2)
						//	16					//	96 (row 3)
						//	16					//	112 (row 4)
};
uniform mat4 model;


out vec3 fNormal;
out vec2 fTexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
  	fTexCoord = vTexCoord;
  	fNormal = vNormal;
}