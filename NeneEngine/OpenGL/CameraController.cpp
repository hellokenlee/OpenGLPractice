/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#include "CameraController.h"


bool CameraController::keysPressed[1024] = {0};
bool CameraController::mouseFirstMove = true;
bool CameraController::lock = false;
double CameraController::lastMousePosX = 0;
double CameraController::lastMousePosY = 0;
GLfloat CameraController::lastFrameTime = 0;
GLfloat CameraController::yaw = -90.0f;
GLfloat CameraController::pitch = 0.0f;
Camera CameraController::camera;

double CameraController::moveSpeed = 5.0f;
double CameraController::mouseSensitivity = 0.05f;

const GLenum CameraController::polygonModes[] = {GL_FILL, GL_LINE, GL_POINT};
int CameraController::modeIndex = 0;

void CameraController::bindControl(GLFWwindow* const window) {
    //绑定控制
    glfwSetKeyCallback(window, CameraController::keyCallback);
    glfwSetCursorPosCallback(window, CameraController::mouseCallback);
    glfwSetScrollCallback(window, CameraController::scrollCallback);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}


void CameraController::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // 处理退出
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    // 处理视角锁定和鼠标的隐/显
    if(key == GLFW_KEY_L && action == GLFW_PRESS) {
        lock = !lock;
        if(lock) {
            glfwSetCursorPosCallback(window, nullptr);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetCursorPosCallback(window, CameraController::mouseCallback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    // 更改绘制方式
    if(key == GLFW_KEY_M && action == GLFW_PRESS) {
        modeIndex = (modeIndex + 1) % 3;
        glPolygonMode(GL_FRONT_AND_BACK, polygonModes[modeIndex]);
    }
    // 记录按键按下
    if(action == GLFW_PRESS)
        keysPressed[key] = true;
    else if(action == GLFW_RELEASE)
        keysPressed[key] = false;
}
void CameraController::mouseCallback(GLFWwindow* window, double nowX, double nowY) {
    if(mouseFirstMove) {
        lastMousePosX = nowX;
        lastMousePosY = nowY;
        mouseFirstMove = false;
    }
    GLfloat deltaX = nowX - lastMousePosX, deltaY = lastMousePosY - nowY;
    lastMousePosX = nowX;
    lastMousePosY = nowY;
    deltaX *= mouseSensitivity;
    deltaY *= mouseSensitivity;
    yaw += deltaX;
    pitch += deltaY;
    if(pitch > 89.5f)
        pitch = 89.5f;
    else if(pitch < -89.5)
        pitch = -89.5;
    camera.rotate(pitch, yaw);
}
void CameraController::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.zoom(yoffset);
}

void CameraController::update() {

    GLfloat currentFrameTime = glfwGetTime();
    GLfloat deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
    GLfloat distance = moveSpeed * deltaTime;
    if(keysPressed[GLFW_KEY_W])
        camera.moveForward(distance);
    if(keysPressed[GLFW_KEY_S])
        camera.moveBack(distance);
    if(keysPressed[GLFW_KEY_A])
        camera.moveLeft(distance);
    if(keysPressed[GLFW_KEY_D])
        camera.moveRight(distance);
    //
    camera.rotate(pitch, yaw);
}
