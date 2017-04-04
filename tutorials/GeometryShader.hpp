/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef GEOMETRY_SHADER_HPP
#define GEOMETRY_SHADER_HPP
namespace GeometyShader{
GLfloat vertices[]={
    // Positions         // Colors
     0.5f, -0.5f,// 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f,// 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
    -0.5f,  0.5f,// 0.0f,  0.0f, 1.0f, 0.0f,   // Top Left
     0.5f,  0.5f // 0.0f,  1.0f, 0.0f, 0.0f,   // Top Right
};
GLfloat points[] = {
    // Positon2D  //  Colors
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
};
void tutorial(){
    GLFWwindow *window=initWindow("GeometryShader",800,600);
    showEnviroment();

    Shader shader("shaders/GeometryShader/pointAsHouse.vs","shaders/GeometryShader/pointAsHouse.frag");
    shader.addOptionalShader("shaders/GeometryShader/pointAsHouse.geom", GL_GEOMETRY_SHADER);

    GLuint VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STATIC_DRAW);
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(GLvoid*)(2*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS,0,4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
}


};
#endif  //  GEOMETRY_SHADER_HPP
