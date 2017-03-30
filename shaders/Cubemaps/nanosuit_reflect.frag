#version 330 core

in vec3 fNormal;
in vec3 fPosition;
in vec2 fTexCoords;

uniform sampler2D texture_diffuse1;//GL_TEXTURE0
// uniform sampler2D texture_specular1;//GL_TEXTURE1
uniform sampler2D texture_reflection1;//GL_TEXTURE2
uniform samplerCube texture_cube1;//GL_TEXTURE3
uniform vec3 fCameraPos;

out vec4 color;

void main()
{    
    //漫发射颜色
    vec4 diffuse_color = texture(texture_diffuse1, fTexCoords);
    //反射
    vec3 I =  normalize(fPosition - fCameraPos);
    vec3 R = reflect(I, normalize(fNormal));
    //根据反射贴图构造反射因子
    vec4 reflect_ratio = texture(texture_reflection1, fTexCoords);
    float reflect_intensity = reflect_ratio.r * reflect_ratio.a;
    vec4 reflect_color = texture(texture_cube1, R) * reflect_intensity;
    //if(reflect_ratio > 0.1){
    //	reflect_color = texture(texture_cube1, R) * reflect_ratio;
    //}
    // 
    color = diffuse_color + reflect_color;
}

