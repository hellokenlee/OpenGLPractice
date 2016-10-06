/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP
//GL math libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Transformations{
GLfloat vertices[] = {
    // Positions          // Colors           // Texture Coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left
};
GLuint indices[] = {
    0,1,3,
    1,2,3
};
void tutorial(){
    //变换
    glm::mat4 trans;
    trans=glm::rotate(trans,90.0f,glm::vec3(0.0,0.0,1.0));
    trans=glm::scale(trans,glm::vec3(0.5,0.5,0.5));

    //创建窗口
    GLFWwindow *window=initWindow("Transformation",800,600,nullptr);
    showEnviroment();
    //创建Shader
    Shaders::Shader shaderProgram("shaders/Transformations/shader.vs","shaders/Transformations/shader.frag");
    //读入数据
    GLuint VBO,VAO,EBO;
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=Textures::loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);
    GLuint tex1=Textures::loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_REPEAT,GL_LINEAR);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //变换
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.programID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return;
}


};      //  namespace Transformations
#endif  //  TRANSFORMATIONS
