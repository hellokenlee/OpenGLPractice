/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "Types.h"
#include "ConstantBuffer.h"

//
//	摄像机类
//

class Camera {
public:
	// 构造函数
	Camera();
	// 移动函数
	void moveForward(FLOAT const distance);
	void moveBack(FLOAT const distance);
	void moveLeft(FLOAT const distance);
	void moveRight(FLOAT const distance);
	void moveUp(FLOAT const distance);
	void moveDown(FLOAT const distance);
	void moveTo(Vec3 pos);
	// 设置视椎体
	void setPerspective(FLOAT const fov, FLOAT const ratio, FLOAT const nearPlane, FLOAT const farPlane);
	// 转动摄像头
	void rotate(FLOAT pitch, FLOAT yaw);
	// 使用该相机
	void use();
public:
	// 视矩阵
	Mat4 view;
	// 投影矩阵
	Mat4 projection;
	// 摄像机位置
	Vec3 position;
	// 摄像机前方
	Vec3 front;
private:
	// 投影相关
	FLOAT fov, ratio, nearPlane, farPlane;
	// 摄像机左方
	Vec3 right;
	// 摄像机上方
	Vec3 up;
};
