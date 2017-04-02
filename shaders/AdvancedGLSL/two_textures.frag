#version 330 core
in vec2 fTexCoords;

out vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{             
    if(gl_FrontFacing){
    	color = texture(texture0, fTexCoords);
    }else{
    	color = texture(texture1, fTexCoords);
    }
}