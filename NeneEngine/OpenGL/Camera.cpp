/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#include "Camera.h"

Camera::Camera() {
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);      //摄像机位置
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   //摄像机前方(摄像机永远注视着摄像机的以摄像机为原点 0,0,-1的 位置)
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       //摄像机的上方向(喂！不许歪着头看东西!
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    view = glm::lookAt(
               cameraPos,//摄像机位置
               cameraPos + cameraFront, //摄像机需要看到的位置//正前方
               cameraUp//摄像机的正上方（需要指定上方的原因是，在同一个位置看同一个地方，头可以歪）
           );
    pfov = 45;
    pratio = 4.0 / 3.0;
    pnear = 0.1;
    pfar = 200.0f;
    projection = glm::perspective(pfov, pratio, pnear, pfar);
}
//移动
void Camera::moveForward(GLfloat const distance) {
    cameraPos += distance * cameraFront;
    update();
}
void Camera::moveBack(GLfloat const distance) {
    cameraPos -= distance * cameraFront;
    update();
}
void Camera::moveRight(GLfloat const distance) {
    cameraPos += distance * cameraRight;
    update();
}
void Camera::moveLeft(GLfloat const distance) {
    cameraPos -= distance * cameraRight;
    update();
}
void Camera::moveto(glm::vec3 const position) {
    cameraPos = position;
    update();
}
//转动摄像头
void Camera::rotate(GLfloat const pitch, GLfloat const yaw) {
    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(cameraFront);
    update();
}
//放大视角
void Camera::zoom(GLfloat const angle) {
    GLfloat fov = pfov - angle;
    if(fov >= 1.0f && fov <= 45.0f) {
        pfov = fov;
        projection = glm::perspective(pfov, pratio, pnear, pfar);
    }
}
void Camera::setPerspective(GLfloat const fov, GLfloat const r, GLfloat const near, GLfloat const far) {
    pfov = fov;
    pratio = r;
    pnear = near;
    pfar = far;
    projection = glm::perspective(pfov, pratio, pnear, pfar);
}
//更新函数
void Camera::update() {
    view = glm::lookAt(
               cameraPos,//摄像机位置
               cameraPos + cameraFront, //摄像机需要看到的位置//正前方
               cameraUp//摄像机的正上方（需要指定上方的原因是，在同一个位置看同一个地方，头可以歪）
           );
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}
//返回View矩阵
const GLfloat* Camera::getViewMatrixVal() const {
    return glm::value_ptr(view);
}

//返回View矩阵
const GLfloat* Camera::getProjectionMatrixVal() const {
    return glm::value_ptr(projection);
}

const GLfloat* Camera::getPositionVal() const {
    return glm::value_ptr(cameraPos);
}
