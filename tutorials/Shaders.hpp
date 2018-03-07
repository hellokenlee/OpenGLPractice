/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef SHADERS_CPP
#define SHADERS_CPP

#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


//Chapcter2 namespace
namespace Shaders{
//顶点数据（位置+颜色）
GLfloat vertices[]={
    // Positions         // Colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top
};
//教程实现:绘制一个彩色三角
void tutorial(){
    //初始化窗口
    GLFWwindow *window=initWindow("Shaders", 800, 600);
    showEnviroment();
    //创建ShaderProgram
    Shader shaderProgram("Resources/Shaders/Shaders/shader.vs","Resources/Shaders/Shaders/shader.frag");
    //导入数据，创建VAO
    GLuint VBO,VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //轮询鼠标或者键盘等输入
        glfwPollEvents();
        //背景色
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //绘制
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);
        //双缓冲置换
        glfwSwapBuffers(window);
    }
    //终止
    glfwTerminate();
    return;
}

//练习1：在shader上另三角形倒转
void exercise1(){
    GLFWwindow *window=initWindow("Shader-EX1", 800, 600);
    showEnviroment();
    Shader shaderProgram("Resources/Shaders/Shaders/shaderEx1.vs","Resources/Shaders/Shaders/shader.frag");
    GLuint VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        //背景色
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //
        shaderProgram.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return;
}

//练习2：令三角形向右偏移任意值
//(uniform 实现)
void exercise2(){
    //FPS计算
    double lastTime=glfwGetTime(),currentTime=glfwGetTime();
    int nbFrames=0;
    //偏移值
    GLfloat offset=0.4;
    GLint offsetLocation;
    //初始化
    GLFWwindow *window=initWindow("Shader-EX2", 800, 600);
    //关闭垂直同步
    glfwSwapInterval(0);
    showEnviroment();
    Shader shaderProgram("Resources/Shaders/Shaders/shaderEx2.vs","Resources/Shaders/Shaders/shader.frag");
    GLuint VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        //背景色
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //更新uniform
        offset=sin(currentTime);
        offsetLocation=glGetUniformLocation(shaderProgram.programID,"offset");
        glUniform1f(offsetLocation,offset);
        //绘制
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);
        //置换双缓冲
        glfwSwapBuffers(window);
        //Count FPS
        currentTime=glfwGetTime();
        ++nbFrames;
        if(currentTime-lastTime >=1.0){
            printf("%f ms/frame\n",1000.0/double(nbFrames));
            nbFrames=0;
            lastTime+=1.0;
        }
    }
    glfwTerminate();
}

//练习3： 把vShader的位置当做fShader的颜色值来绘制
//Q: why is the bottom-left side of our triangle black?
//A: 因为3个定点的z坐标都为0，因此RGB的B值一直为0，又因为左下角的顶点在第三象限，因此x,y值都小于0，所以r,g值都小于0，直接被当做0处理。
void exercise3(){
    //初始化
    GLFWwindow *window=initWindow("Shader-EX3", 800, 600);
    showEnviroment();
    Shader shaderProgram("Resources/Shaders/Shaders/shaderEx3.vs","Resources/Shaders/Shaders/shader.frag");
    GLuint VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    checkError();
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        //背景色
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //绘制
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);
        //置换双缓冲
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

};      ////namespace Shaders
#endif  //SHADERS_HPP
