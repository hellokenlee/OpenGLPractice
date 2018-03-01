#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 移除摄像机view矩阵的平移量，等效于让天空盒跟随玩家移动
    mat3 view3x3 = mat3(view);
    // 从3x3view构造4x4view
    // 优化，把天空盒的深度值永远设为1,即最远深度
    vec4 pos = projection * mat4(view3x3) * model * vec4(position, 1.0f);
    // 把z值设为w,即1.0;
    gl_Position = pos.xyww;
    // 3D纹理坐标等于其坐标
    TexCoords = position;
}