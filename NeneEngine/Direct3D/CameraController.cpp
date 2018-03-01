/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "CameraController.h"

//
HWND CameraController::hWnd = 0;
Camera* CameraController::pCamera = nullptr;
Camera CameraController::defaultCamera;
LPDIRECTINPUT8 CameraController::pDirectInput = nullptr;
IDirectInputDevice8* CameraController::pKeyboard = nullptr;
IDirectInputDevice8* CameraController::pMouse = nullptr;
DIMOUSESTATE CameraController::prevMouseState = {}; 
DIMOUSESTATE CameraController::currMouseState = {};
BYTE CameraController::keyStatus[256] = {};
BOOL CameraController::keyPressed[256] = {};
FLOAT  CameraController::pitch = 0.0f, CameraController::yaw = -90.0f;
FLOAT CameraController::moveSpeed = 0.02f, CameraController::mouseSensitivity = 0.001f;
BOOL CameraController::isBloom = false;
int CameraController::caStatus = 0;
BOOL CameraController::aboardStatus = 0;
//
void (*CameraController::pKeyboardCallback)(BYTE const (&keyStatus)[256]) = CameraController::defaultKeyboardCallback;
void (*CameraController::pMouseCallback)(LONG const IX, LONG const IY, LONG const IZ, BYTE const (&buttonPressed)[4]) = CameraController::defaultMouseCallback;

//
void CameraController::defaultKeyboardCallback(BYTE const (&keyStatus)[256]) {
	// w,a,s,d 控制方向
	if (keyStatus[DIK_W] & 0x80) {
		pCamera->moveForward(moveSpeed);
	}
	if (keyStatus[DIK_S] & 0x80) {
		pCamera->moveBack(moveSpeed);
	}
	if (keyStatus[DIK_A] & 0x80) {
		pCamera->moveLeft(moveSpeed);
	}
	if (keyStatus[DIK_D] & 0x80) {
		pCamera->moveRight(moveSpeed);
	}
	// esc 退出
	if (keyStatus[DIK_ESCAPE] & 0x80) {
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	}
	// m 更改绘制模式
	if (keyStatus[DIK_M] & 0x80 && !keyPressed[DIK_M]) {
		UINT mode = ((UINT)Utils::getPolygonMode() + 1) % (UINT)POLYGON_MODES_NUM;
		Utils::setPolygonMode((PolygonMode)mode);
	}
	if (keyStatus[DIK_M] & 0x80) {
		keyPressed[DIK_M] = true;
	} else {
		keyPressed[DIK_M] = false;
	}
	// b 显示泛光
	if (keyStatus[DIK_B] & 0x80 && !keyPressed[DIK_B]) {
		isBloom = !isBloom;
	}
	if (keyStatus[DIK_B] & 0x80) {
		keyPressed[DIK_B] = true;
	} else {
		keyPressed[DIK_B] = false;
	}
	// c 控制坐标系
	if (keyStatus[DIK_C] & 0x80 && !keyPressed[DIK_C]) {
		++caStatus;
		caStatus = caStatus >= 3 ? 0 : caStatus;
	}
	if (keyStatus[DIK_C] & 0x80) {
		keyPressed[DIK_C] = true;
	} else {
		keyPressed[DIK_C] = false;
	}
	// c 控制坐标系
	if (keyStatus[DIK_F] & 0x80 && !keyPressed[DIK_F]) {
		if (aboardStatus == 2) {
			// 下车
			aboardStatus = 1;
		}else if (aboardStatus == 1) {
			// 上车
			aboardStatus = 2;
		}
	}
	if (keyStatus[DIK_F] & 0x80) {
		keyPressed[DIK_F] = true;
	}
	else {
		keyPressed[DIK_F] = false;
	}
}

void CameraController::defaultMouseCallback(LONG const deltaX, LONG const deltaY, LONG const deltaZ, BYTE const (&buttonPressed)[4]) {
	// 更新pitch, yaw
	yaw += (mouseSensitivity * deltaX);
	pitch -= (mouseSensitivity * deltaY);
	// 防止越界
	if (pitch > M_PI_OVER_2 - 0.01f)
		pitch = M_PI_OVER_2 - 0.01f;
	else if (pitch < -M_PI_OVER_2 + 0.01f)
		pitch = -M_PI_OVER_2 + 0.01f;
	if (yaw > M_PI_TIMES_2)
		yaw -= M_PI_TIMES_2;
	else if (yaw < -M_PI_TIMES_2)
		yaw += M_PI_TIMES_2;
	// 对相机执行操作
	pCamera->rotate(pitch, yaw);
}

bool CameraController::bindControll(HINSTANCE hInstance, HWND _hWnd) {
	//
	hWnd = _hWnd;
	defaultCamera = Camera();
	pCamera = &defaultCamera;
	// 初始化DirectInput
	HRESULT hr1 = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, nullptr);
	checkResult(hr1, "Failed to init Direct Input!");
	// 初始化键盘设备
	HRESULT hr2 = pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboard, nullptr);
	checkResult(hr2, "Failed to init keyboard!");
	if (SUCCEEDED(hr2)) {
		pKeyboard->SetDataFormat(&c_dfDIKeyboard);
		pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	}
	// 初始化鼠标设备
	HRESULT hr3 = pDirectInput->CreateDevice(GUID_SysMouse, &pMouse, nullptr);
	checkResult(hr3, "Failed to init mouse!");
	if (SUCCEEDED(hr3)) {
		pMouse->SetDataFormat(&c_dfDIMouse);
		pMouse->SetCooperativeLevel(hWnd, DISCL_NOWINKEY | DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	}
	//
	return SUCCEEDED(hr1) && SUCCEEDED(hr2) && SUCCEEDED(hr3);
}

void CameraController::clear() {
	if (pKeyboard) pKeyboard->Unacquire();
	if (pMouse) pMouse->Unacquire();
	if (pDirectInput) pDirectInput->Release();
}

void CameraController::pollInput() {
	// 获取当前键盘状态
	pKeyboard->Acquire();
	pKeyboard->GetDeviceState(sizeof(keyStatus), (LPVOID)&keyStatus);
	// 获取当前鼠标状态
	pMouse->Acquire();
	pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &currMouseState);
	// 调用键盘回调
	if (pKeyboardCallback) pKeyboardCallback(keyStatus);
	// 调用鼠标回调
	if (pMouseCallback) pMouseCallback(currMouseState.lX, currMouseState.lY, currMouseState.lZ, currMouseState.rgbButtons);
}

