/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef SHADERS_HPP
#define SHADERS_HPP
namespace shaders{};
#include <math.h>
void shaders(){
    //数据
    const GLchar *vertexShaderSource=
        "#version 330 core\n"
        "layout(location=0) in vec3 position;\n"
        "layout(location=1) in vec3 color;"
        "out vec3 vectexColor;"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(position,1.0);\n"
        "   vectexColor=color;"
        "}\n"
    ;
    const GLchar *fragmentShaderSource=
        "#version 330 core\n"
        "in vec3 vectexColor;"
        "out vec4 color;"
        "void main()\n"
        "{\n"
        "   color=vec4(vectexColor,1.0f);"
        "}\n"
    ;
    GLfloat vertices[]={
        // Positions         // Colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top
    };
    //初始化窗口
    GLFWwindow *window=initWindow("HelloKenLee!",800,600);
    showEnviroment();

    //创建ShaderProgram
    //  vertex shader
    GLuint vShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader,1,&vertexShaderSource,nullptr);
    glCompileShader(vShader);
    checkShaderCompile(vShader);
    //  fragment shader
    GLuint fShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader,1,&fragmentShaderSource,nullptr);
    glCompileShader(fShader);
    checkShaderCompile(fShader);
    //  shader program
    GLuint sProgram=glCreateProgram();
    glAttachShader(sProgram,vShader);
    glAttachShader(sProgram,fShader);
    glLinkProgram(sProgram);
    checkProgramLink(sProgram);
    //  delete shader
    glDeleteShader(vShader);
    glDeleteShader(fShader);
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
        //背景色
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //绘制
        glUseProgram(sProgram);
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





#endif
