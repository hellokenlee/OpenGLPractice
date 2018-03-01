/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef Coordinate_Axes_H
#define Coordinate_Axes_H

#include "Camera.h"
#include "Shader.h"


//
//	画x,y,z三色坐标轴
//
class CoordinateAxes {
public:
    // 构造函数
    CoordinateAxes(Camera *cam);
    // 析构函数
    ~CoordinateAxes();
    // 画出坐标系，主循环调用
    void draw();
    // 2倍增加坐标轴长度
    void extend();
    // 是否显示网格
    void showGrid(bool const flag);
    // 是否显示坐标轴
    void showAxes(bool const flag);
public:
    // 是否绘制网格
    bool grid;
    // 是否显示坐标轴
    bool axes;
private:
    //禁止构造空
    CoordinateAxes();
    //坐标轴绘制的最大长度
    GLfloat len;
    //绘制坐标轴的着色器
    std::unique_ptr<Shader> axeShader;
    std::unique_ptr<Shader> gridShader;
    //摄像头，由外部传入
    Camera *cam;
    //网格间距
    GLfloat gap;
    //一些显存数据
    GLuint axeVAO, axeVBO;
    GLuint gridVAO, gridVBO;
    GLuint axeViewPos, axeProjPos, gridViewPos, gridProjPos;
    GLuint gridVertexNums;
};


#endif	//	Coordinate_Axes
