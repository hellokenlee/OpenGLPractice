/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "Utils.h"
#include "FPSCounter.h"
#include "Camera.h"
#include "CameraController.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"

//
//	GUI控制面板
//

class Panel {
public:
	// 构造函数
	Panel();
	// 析构函数
	~Panel();
	// 绘制函数
	void draw();
private:
	FPSCounter fc;
	Camera *pCamera;
};