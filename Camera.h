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

class Camera(){
public:
	//构造函数
	Camera();
	//摄像机移动
	void moveForward(GLuint distance);
	void moveBack(GLuint distance);
	void moveRight(GLuint distance);
	void moveLeft(GLuint distance);
	//转动摄像头
	void rotate(int pitch, int yaw);
	//放大视角
	void zoom(int angle);
	//返回View矩阵
	glm::mat4 getViewMatrix();
	//更新函数
	void update();
private:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);      //摄像机位置
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   //摄像机前方(摄像机永远注视着摄像机的以摄像机为原点 0,0,-1的 位置)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       //摄像机的上方向(喂！不许歪着头看东西!)
};

#endif	// CAMERA_H