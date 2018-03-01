/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "Shader.h"
#include "Camera.h"
#include "Shape.h"
#include <vector>
//
//	坐标轴类，显示坐标轴
//

class CoordinateAxes {
public:
	// 构造
	CoordinateAxes();
	// 析构
	~CoordinateAxes();
	// 绘制函数
	void draw(Camera* const cam = nullptr);
	// 延长至原来绘制的两倍长
	void extend();
public:
	// 是否绘制网格
	bool isDrawGrid;
	// 是否绘制XYZ三个坐标轴
	bool isDrawAxes;
private:
	// 禁止拷贝构造
	CoordinateAxes(const CoordinateAxes& other);
	// 最大长度， 间隔长度
	FLOAT mLen, gapLen;
	// 两个着色器
	Shader *pAxesShader, *pGridShader;
	// 顶点缓冲指针
	Shape *pAxes, *pGrid;
};