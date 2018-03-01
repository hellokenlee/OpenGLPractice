/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "Camera.h"

//
//	控制器类
//

class CameraController {
public:
	// 初始化
	static bool bindControll(HINSTANCE hInstance, HWND hWnd);
	// 清理
	static void clear();
	// 处理输入，每帧调用
	static void pollInput();
	// 默认键盘处理函数
	static void defaultKeyboardCallback(BYTE const (&keyPressed)[256]);
	// 默认鼠标处理函数
	static void defaultMouseCallback(LONG const IX, LONG const IY, LONG const IZ, BYTE const (&buttonPressed)[4]);
public:
	// 移动速度 [default = 0.01f]
	static FLOAT moveSpeed;
	// 鼠标灵敏度 [default = 0.001f]
	static FLOAT mouseSensitivity;
	// 摄像机指针
	static Camera* pCamera;
	// 俯仰角, 偏航角
	static FLOAT yaw, pitch;
	//
	static BOOL isBloom;
	static int caStatus;
	static int aboardStatus;
private:
	// 窗口句柄
	static HWND hWnd;
	// 默认的摄像机
	static Camera defaultCamera;
	// DI设备指针
	static LPDIRECTINPUT8 pDirectInput;
	// 键盘设备
	static IDirectInputDevice8* pKeyboard;
	// 鼠标设备
	static IDirectInputDevice8* pMouse;
	// 上一次鼠标的状态
	static DIMOUSESTATE prevMouseState, currMouseState;
	// 键盘记录
	static BYTE keyStatus[256];
	static BOOL keyPressed[256];
	// 每帧调用的鼠标键盘回调
	static void(*pKeyboardCallback)(BYTE const (&keyPressed)[256]);
	static void(*pMouseCallback)(LONG const deltaX, LONG const deltaY, LONG const deltaZ, BYTE const (&buttonPressed)[4]);
};