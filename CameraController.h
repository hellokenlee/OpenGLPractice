/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
#include <stdio.h>
//Camera
#include "Camera.h"
//
//	Camera控制器类
//

class CameraController{
public:
    //键盘回调函数
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	//鼠标移动回调函数
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	//鼠标滚轮回调函数
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void update();
    //Camera对象
	static Camera camera;
    //移动速度
	static double moveSpeed;
    //鼠标灵敏度
    static double mouseSensitivity;
private:
    //单例模式禁止构造/拷贝构造
	CameraController();
	CameraController(const CameraController& cc);
	CameraController& operator=(const CameraController& cc);
private:
	//按键记录
	static bool keysPressed[1024];
	//鼠标位置
	static double lastMousePosX,lastMousePosY;
	//上一帧时间
	static GLfloat lastFrameTime;
	//鼠标转动角
    static GLfloat yaw,pitch;
    static bool mouseFirstMove;

};


#endif	//	CAMERA_CONTROLLER_H
