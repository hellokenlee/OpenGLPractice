/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef SHADERS_HPP
#define SHADERS_HPP
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//Chapcter2 namespace
namespace Shaders{
//Shader类
class Shader{
public:
    GLuint programID;
    //构造函数
    Shader(const GLchar* vShaderSourcePath,const GLchar* fShaderSourcePath){
        std::string vShaderSource;
        std::string fShaderSource;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        try {
            //读入Shader文件
            vShaderFile.open(vShaderSourcePath);
            fShaderFile.open(fShaderSourcePath);
            std::stringstream vShaderStream,fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vShaderSource=vShaderStream.str();
            fShaderSource=fShaderStream.str();
        } catch(std::ifstream::failure e) {
            //处理异常
            std::cout<<"Error::Can not read shader source file!"<<std::endl;
            std::cout<<e.what()<<endl;
            this->programID=0;
            return;
        }
        const GLchar* vShaderCode=vShaderSource.c_str();
        const GLchar* fShaderCode=fShaderSource.c_str();
        //编译Shader
        GLuint vShader,fShader;
        vShader=glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader,1,&vShaderCode,nullptr);
        glCompileShader(vShader);
        checkShaderCompile(vShader);
        fShader=glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader,1,&fShaderCode,nullptr);
        glCompileShader(fShader);
        checkShaderCompile(fShader);
        //链接Shader程序
        this->programID=glCreateProgram();
        glAttachShader(this->programID,vShader);
        glAttachShader(this->programID,fShader);
        glLinkProgram(this->programID);
        checkProgramLink(this->programID);
        //释放Shder占用的空间
        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }
    //使用ShaderProgram(用于MainLoop)
    void use(){
        glUseProgram(this->programID);
    }
private:
    //禁止默认构造函数
    Shader(){
        programID=0;
    }
    //检查GLSL编译情况
    void checkShaderCompile(GLuint shaderID){
        GLint isSuccess;
        glGetShaderiv(shaderID,GL_COMPILE_STATUS,&isSuccess);
        if(!isSuccess){
            GLchar infoLog[512];
            glGetShaderInfoLog(shaderID,512,nullptr,infoLog);
            cout<<"ERROR: Comlile Shader Error:"<<endl<<infoLog<<endl;
            exit(-1);
        }
    }
    //检查多Shader链接情况
    void checkProgramLink(GLuint programID){
        GLint isSuccess;
        glGetProgramiv(programID,GL_LINK_STATUS,&isSuccess);
        if(!isSuccess){
            GLchar infoLog[512];
            glGetProgramInfoLog(programID,512,nullptr,infoLog);
            cout<<"ERROR: Link Program Error:"<<endl<<infoLog<<endl;
            exit(-1);
        }
    }
};

//顶点数据（位置+颜色）
GLfloat vertices[]={
    // Positions         // Colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top
};
//教程实现
void tutorial(){
    //初始化窗口
    GLFWwindow *window=initWindow("Shaders",800,600);
    showEnviroment();
    //创建ShaderProgram
    Shader shaderProgram("shaders/Shaders/shader.vs","shaders/Shaders/shader.frag");
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

//练习1
void exercise1(){
    return;
}

};
#endif
