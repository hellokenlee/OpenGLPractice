#version 330 core
in vec2 fTexCoords;

out vec4 color;

layout (std140) uniform Samplers
{
	// 					// alignment size	offset
	sampler2D texture0; // 		4				0
	sampler2D texture1; //		4				4
};

void main()
{             
    if(gl_FrontFacing){
    	color = texture(texture0, fTexCoords);
    }else{
    	color = texture(texture1, fTexCoords);
    }
}