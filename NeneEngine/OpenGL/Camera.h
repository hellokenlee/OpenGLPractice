/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef CAMERA_H
#define CAMERA_H

// GL
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//
//	Camera类：实现了摄像机漫游
//

class Camera {
public:
    // 构造函数
    Camera();
    // 摄像机移动
    void moveForward(GLfloat const distance);
    void moveBack(GLfloat const distance);
    void moveRight(GLfloat const distance);
    void moveLeft(GLfloat const distance);
    void moveto(glm::vec3 const positon);
    // 转动摄像头
    void rotate(GLfloat const pitch, GLfloat const yaw);
    // 放大视角
    void zoom(GLfloat const angle);
    // 设置视角参数
    void setPerspective(GLfloat const fov, GLfloat const r, GLfloat const near, GLfloat const far);
    // 返回View矩阵
    const GLfloat* getViewMatrixVal() const;
    // 返回Projection矩阵（透视模式ONLY）
    const GLfloat* getProjectionMatrixVal() const;
    // 返回摄像机的位置
    const GLfloat* getPositionVal() const;
public:
    // 更新函数
    void update();
public:
    // 视椎体参数
    GLfloat pfov, pratio, pnear, pfar;
    // 摄像机位置
    glm::vec3 cameraPos;
    // 摄像机前方
    glm::vec3 cameraFront;
    // 摄像机右方
    glm::vec3 cameraRight;
    // 摄像机的上方向
    glm::vec3 cameraUp;
    // view矩阵
    glm::mat4 view;
    // 投影矩阵
    glm::mat4 projection;
};

#endif	// CAMERA_H
