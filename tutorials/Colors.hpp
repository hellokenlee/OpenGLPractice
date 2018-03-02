/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef COLORS_CPP
#define COLORS_CPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace Colors{
//顶点信息前置声明
extern GLfloat cubeVertices[36*5];
//
float cubeColor[]={1.0f, 0.5f, 0.31f};
float lampColor[]={1.0f,1.0f,1.0f};
glm::vec3 cubePos(0.0f,0.5f,0.0f);
glm::vec3 lampPos(1.2f,1.5f,-2.0f);
//教程实现
void tutorial(){
    GLFWwindow *window=initWindow("Colors",800,600);
    showEnviroment();
    Camera* pCamera = CameraController::getCamera();
    //盒子顶点数据
    GLuint cubeVBO,cubeVAO;
    glGenBuffers(1,&cubeVBO);
    glGenVertexArrays(1,&cubeVAO);
    glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    //光源顶点数据
    GLuint lampVAO;
    glGenVertexArrays(1,&lampVAO);
    glBindVertexArray(lampVAO);
        glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    //着色器程序
    Shader cubeShader("Resources/Shaders/Colors/shader.vs","Resources/Shaders/Colors/cubefrag.frag");
    Shader lampShader("Resources/Shaders/Colors/shader.vs","Resources/Shaders/Colors/lampfrag.frag");
    //uniform 的位置
    GLint cubeColorPos=glGetUniformLocation(cubeShader.programID,"objColor");
    GLint lightColorPos=glGetUniformLocation(cubeShader.programID,"lightColor");
    GLint cubeModelPos=glGetUniformLocation(cubeShader.programID,"model");
    GLint cubeViewPos=glGetUniformLocation(cubeShader.programID,"view");
    GLint cubeProjPos=glGetUniformLocation(cubeShader.programID,"projection");

    GLint lampColorPos=glGetUniformLocation(lampShader.programID,"lampColor");
    GLint lampModelPos=glGetUniformLocation(lampShader.programID,"model");
    GLint lampViewPos=glGetUniformLocation(lampShader.programID,"view");
    GLint lampProjPos=glGetUniformLocation(lampShader.programID,"projection");
    //静态设置uniform
    cubeShader.use();
    glUniform3f(cubeColorPos,cubeColor[0],cubeColor[1],cubeColor[2]);
    glUniform3f(lightColorPos,lampColor[0],lampColor[1],lampColor[2]);
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, cubePos);
    glUniformMatrix4fv(cubeModelPos,1,GL_FALSE,glm::value_ptr(model));

    lampShader.use();
    glUniform3f(lampColorPos,lampColor[0],lampColor[1],lampColor[2]);
    model = glm::mat4();
    model = glm::translate(model, lampPos);
    model = glm::scale(model, glm::vec3(0.2f));
    glUniformMatrix4fv(lampModelPos,1,GL_FALSE,glm::value_ptr(model));
    //绑定控制
    glfwSetKeyCallback(window,CameraController::keyCallback);
    glfwSetCursorPosCallback(window,CameraController::mouseCallback);
    glfwSetScrollCallback(window,CameraController::scrollCallback);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(CameraController::getCamera());
    //帧数计数器
    FPSCounter fc;
    //Main loop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lampShader.use();
        glUniformMatrix4fv(lampViewPos,1,GL_FALSE, pCamera->getViewMatrixVal());
        glUniformMatrix4fv(lampProjPos,1,GL_FALSE, pCamera->getProjectionMatrixVal());
        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        cubeShader.use();
        glUniformMatrix4fv(cubeViewPos,1,GL_FALSE, pCamera->getViewMatrixVal());
        glUniformMatrix4fv(cubeProjPos,1,GL_FALSE, pCamera->getProjectionMatrixVal());
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);

        fc.update();
    }
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glfwTerminate();
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
};
#endif  //  COLORS_HPP
