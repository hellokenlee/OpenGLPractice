/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef CAMERA_H
#define CAMERA_H

//GL
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//
//	Camera类：实现了摄像机漫游
//

class Camera{
public:
	//构造函数
	Camera();
	//摄像机移动
	void moveForward(GLfloat distance);
	void moveBack(GLfloat distance);
	void moveRight(GLfloat distance);
	void moveLeft(GLfloat distance);
	void moveto(glm::vec3 positon);
	//转动摄像头
	void rotate(GLfloat pitch, GLfloat yaw);
	//放大视角
	void zoom(GLfloat angle);
	//返回View矩阵
	GLfloat* getViewMatrixVal();
	GLfloat getFov();
private:
    //更新函数
	void update();
private:
	GLfloat fov;
	glm::vec3 cameraPos;//摄像机位置
	glm::vec3 cameraFront;//摄像机前方
	glm::vec3 cameraRight;//摄像机右方
	glm::vec3 cameraUp;//摄像机的上方向
	glm::mat4 view;//矩阵
};

#endif	// CAMERA_H
