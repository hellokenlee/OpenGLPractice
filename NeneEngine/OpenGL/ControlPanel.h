/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H
// GL
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
// wmdge
#include "FPSCounter.h"
#include "CameraController.h"
//
#include <iostream>

//
//	ControlPanel类： 实现了图形界面的控制面板
//

class ControlPanel{
public:
    // 构造函数
    ControlPanel(GLFWwindow *window);
    // 析构函数
    ~ControlPanel();
    // 绘制函数
    void draw();
};
#endif// CONTROL_PANEL_H
