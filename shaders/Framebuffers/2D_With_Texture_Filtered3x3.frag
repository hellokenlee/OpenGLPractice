#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture1;

const float offset=1.0f/300;


void main()
{             
	//从8邻域采样，邻域定义为纹理坐标距离1/300的像素值
	vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right
    );

	//锐化
    float kernel[9]=float[](
    	 1.0/9, 1.0/9, 1.0/9, 
    	 1.0/9, 1.0/9, 1.0/9, 
    	 1.0/9, 1.0/9, 1.0/9
    );

    vec3 sampleTex[9];
    for(int i=0;i<9;++i){
    	sampleTex[i]=vec3(texture(texture1,TexCoords.st+offsets[i]));
    }
    vec3 res=vec3(0.0f);
    for(int i=0;i<9;++i){
    	res+=sampleTex[i]*kernel[i];
    }
    color=vec4(res,1.0f);
    //color=texture(texture1,TexCoords);
}