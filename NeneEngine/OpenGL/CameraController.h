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
//	Camera控制器静态类
//

class CameraController {
public:
    // 移动速度
    static double moveSpeed;
    // 鼠标灵敏度
    static double mouseSensitivity;
public:
    // 控制绑定
    static void bindControl(GLFWwindow* const window);
    // 键盘回调函数
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    // 鼠标移动回调函数
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    // 鼠标滚轮回调函数
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    // 更新函数
    static void update();
    //
    static inline Camera* getCamera() {
        return &camera;
    }
public:
    // 按键记录
    static bool keysPressed[1024];
    // 鼠标位置
    static double lastMousePosX, lastMousePosY;
    // 上一帧时间
    static GLfloat lastFrameTime;
    // 鼠标转动角
    static GLfloat yaw, pitch;
    // 忽略鼠标第一次移动
    static bool mouseFirstMove;
    // 是否锁定摄像机
    static bool lock;
    //
    static const GLenum polygonModes[];
    //
    static int modeIndex;
private:
    //禁止任何形式的构造
    CameraController();
    CameraController(const CameraController& cc);
    CameraController& operator=(const CameraController& cc);
    //
    static Camera camera;

};


#endif	//	CAMERA_CONTROLLER_H
