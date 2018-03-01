/*Copyleft reserved by KenLee@2018 hellokenlee@163.com*/
#include "Camera.h"

using namespace DirectX::SimpleMath;// 该库为右手坐标系 ( 太好了。

Camera::Camera() {
	// 初始化视矩阵
	position = Vec3(0.0f, 0.0f, 3.0f);
	front = Vec3(0.0f, 0.0f, -1.0f);
	up = Vec3(0.0f, 1.0f, 0.0f);
	right = front.Cross(up);
	right.Normalize();
	view = Matrix::CreateLookAt(position, position + front, up);
	// 初始化投影矩阵
	fov = radians(45.0f);
	ratio = (FLOAT)Utils::getScreenWidth() / (FLOAT)Utils::getScreenHeight();
	nearPlane = 0.1f;
	farPlane = 1000.0f;
	projection = Matrix::CreatePerspectiveFieldOfView(fov, ratio, nearPlane, farPlane);
}

void Camera::rotate(FLOAT pitch, FLOAT yaw) {
	// 更新前向量
	front.x = cosf(yaw) * cosf(pitch);
	front.y = sinf(pitch);
	front.z = sinf(yaw) * cosf(pitch);
	// 单位化
	front.Normalize();
	// 更新View矩阵
	view = Matrix::CreateLookAt(position, position + front, up);
	// 更新左方向向量
	right = front.Cross(up);
	right.Normalize();
}

void Camera::moveForward(FLOAT const distance) {
	position += distance * front;
}

void Camera::moveBack(FLOAT const distance) {
	position -= distance * front;
}

void Camera::moveLeft(FLOAT const distance) {
	position -= distance * right;
}

void Camera::moveRight(FLOAT const distance) {
	position += distance * right;
}

void Camera::moveUp(FLOAT const distance) {
	position += distance * up;
}

void Camera::moveDown(FLOAT const distance) {
	position -= distance * up;
}

void Camera::moveTo(Vec3 pos) {
	position = pos;
}

void Camera::setPerspective(FLOAT const fov, FLOAT const ratio, FLOAT const nearPlane, FLOAT const farPlane) {
	projection = Matrix::CreatePerspectiveFieldOfView(fov, ratio, nearPlane, farPlane);
}

void Camera::use() {
	// 更新View矩阵
	view = Matrix::CreateLookAt(position, position + front, up);
	// 写入Constant Buffer
	ConstantBuffer::perFrameCBData.view = view.Transpose();
	ConstantBuffer::perFrameCBData.cameraPosition = position;
	ConstantBuffer::perFrameCBData.projection = projection.Transpose();
}
