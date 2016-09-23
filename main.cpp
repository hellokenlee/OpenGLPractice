//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//C++ Library
#include <cstdio>
#include <cstdlib>
#include <iostream>
//Tool set
#include "utils.hpp"

using namespace std;

//所有顶点数据
GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,  // Top Right
     0.5f, -0.5f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f,  // Bottom Left
    -0.5f,  0.5f, 0.0f   // Top Left
};
//绘制顶点顺序
GLuint indices[]={
    0,1,3,
    1,2,3
};
//vertexShader GLSL
const GLchar *vertexShaderSource=
    "#version 330 core\n"
    "layout (location=0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
//fragmentShader GLSL
const GLchar *fragmentShaderSource=
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color=vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}\n";
//初始化窗口
GLFWwindow* initWindow(const char* window_name,int w,int h){
    //初始化GLFW。设置GLFW参数，GL版本3.3，只使用CORE_PROFILE，不能Resize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
    //创建窗口
    GLFWwindow *window = glfwCreateWindow(w,h,window_name,nullptr,nullptr);
    //检查是否成功
    if(window == nullptr){
        cout<<"ERROR: Fail to create GLFW window"<<endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    //初始化GLEW。 使用Experimental函数允许我们更好的使用core_profile
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){
        cout<<"ERROR: Fail to init GLEW"<<endl;
        exit(-1);
    }
    //获取当前窗口大小，设置GLVIewport
    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);
    return window;
}
//检查GLSL编译情况
void checkShaderCompile(GLuint shaderID){
    GLint isSuccess;
    glGetShaderiv(shaderID,GL_COMPILE_STATUS,&isSuccess);
    if(!isSuccess){
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID,512,nullptr,infoLog);
        cout<<"ERROR: Comlile VertexShader Error:"<<endl<<infoLog<<endl;
        exit(-1);
    }
}
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
int main(){
    //初始化窗口
    GLFWwindow *window=initWindow("HelloKenLee!",800,600);
    showEnviroment();
    //变量
    //GLfloat r=0.0f,g=0.0f,b=0.0f;
    //创建VBO 申请显存访问
    GLuint VBO;
    glGenBuffers(1,&VBO);
    //创建VAO
    GLuint VAO;
    glGenVertexArrays(1,&VAO);
    //创建EBO
    GLuint EBO;
    glGenBuffers(1,&EBO);
    //绑定VAO,VBO,启用VAO
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);//绑定顶点属性(一般来说，顶点属性包括 位置，法线，颜色，纹理坐标等，这里只有位置属性)
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    //创建顶点Shader
    GLuint vertexShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,nullptr);
    glCompileShader(vertexShader);
    checkShaderCompile(vertexShader);
    //创建fragmentShader
    GLuint fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,nullptr);
    glCompileShader(fragmentShader);
    checkShaderCompile(fragmentShader);
    //创建ShaderProgram
    GLuint shaderProgram=glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLink(shaderProgram);
    //删除Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //主循环
    while(!glfwWindowShouldClose(window)){
        //轮询鼠标或者键盘等输入
        glfwPollEvents();
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //使用Shader程序
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_QUADS,0,4);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //双缓冲置换
        glfwSwapBuffers(window);
    }
    //终止渲染
    glfwTerminate();
    return 0;
}
