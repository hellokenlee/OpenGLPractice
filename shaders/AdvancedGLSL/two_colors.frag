#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture1;

void main()
{             
    if(gl_FragCoord.x < 400){
    	color = vec4(1, 0, 0, 1);
    }else{
    	color = vec4(0, 1, 0, 1);
    }    
}