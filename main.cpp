/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//FreeImage
#include <FreeImage.h>
//C++ Library
#include <cstdlib>
#include <cstdio>
#include <iostream>
//Helper Classes
#include "wmdge/FPSCounter.h"
#include "wmdge/Shader.h"
#include "wmdge/TextureManager.h"
#include "wmdge/Camera.h"
#include "wmdge/CameraController.h"
#include "wmdge/CoordinateAxes.h"
#include "wmdge/Object.h"
#include "wmdge/utils.hpp"
//Tutorial and exercises
#include "tutorials/Benchmark.hpp"
#include "tutorials/HelloTriangle.hpp"
#include "tutorials/Shaders.hpp"
#include "tutorials/Textures.hpp"
#include "tutorials/Transformations.hpp"
#include "tutorials/CoordinateSystem.hpp"
#include "tutorials/Camera.hpp"
#include "tutorials/Colors.hpp"
#include "tutorials/BasicLighting.hpp"

void wmegeTest();

int main(){
    wmegeTest();
    //BasicLighting::tutorial();
    return 0;
}



//顶点信息
GLfloat cubeVertices[36*5] = {
    //position            //texture
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
void wmegeTest(){
    GLFWwindow *window=initWindow("BasicLighting",800,600);
    showEnviroment();
    //着色器程序初始化
    Shader cubeShader("shaders/BasicLighting/shader.vs","shaders/BasicLighting/cubefrag.frag");
    cubeShader.use();
    glUniform3f(glGetUniformLocation(cubeShader.programID,"objColor"),1.0f,0.5f,0.31f);
    //物体对象初始化
    Object cube(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&cubeShader);
    //绑定控制
    glfwSetKeyCallback(window,CameraController::keyCallback);
    glfwSetCursorPosCallback(window,CameraController::mouseCallback);
    glfwSetScrollCallback(window,CameraController::scrollCallback);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //帧数计数器
    FPSCounter fc;
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        ca.draw();
        float y=sin(glfwGetTime());
        cube.moveTo(glm::vec3(0,y,0));
        cube.draw();
        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

