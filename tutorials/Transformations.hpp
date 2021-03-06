/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TRANSFORMATIONS_CPP
#define TRANSFORMATIONS_CPP


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

//教程实现：旋转的盒子
void tutorial(){
    //定义变换矩阵
    //创建窗口
    GLFWwindow *window=initWindow("Transformation", 800, 600);
    showEnviroment();
    //创建Shader
    Shader shaderProgram("Resources/Shaders/Transformations/shader.vs","Resources/Shaders/Transformations/shader.frag");
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
    Texture _tex2("Resources/Textures/face.png", GL_BGRA, GL_RGBA);
    Texture _tex1("Resources/Textures/container.jpg", GL_BGR, GL_RGB);
    GLuint tex2 = _tex2.getTexID();
    GLuint tex1 = _tex1.getTexID();
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //变换矩阵
        glm::mat4 trans;
        trans=glm::translate(trans, glm::vec3(0.75f, -0.75f, 0.0f));
        trans=glm::scale(trans,glm::vec3(0.5,0.5,0.5));
        trans=glm::rotate(trans,(GLfloat)glfwGetTime() * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        //变换
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.programID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        //绑定纹理1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"), 0);
        //绑定纹理2
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"), 1);
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

//练习1：把旋转和平移的顺序换过来会发生什么？为什么？
//A: 盒子会绕着左上角做圆周运动。
//   矩阵操作事实上是逆着的，因此盒子是平移到右下角，然后绕着原点做旋转。看起来就像圆周运动。
void exercise1(){
    //

    //创建窗口
    GLFWwindow *window=initWindow("Transformation-EX1", 800, 600);
    showEnviroment();
    //创建Shader
    Shader shaderProgram("Resources/Shaders/Transformations/shader.vs","Resources/Shaders/Transformations/shader.frag");
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
    Texture _tex2("Resources/Textures/face.png", GL_BGRA, GL_RGBA);
    Texture _tex1("Resources/Textures/container.jpg", GL_BGR, GL_RGB);
    GLuint tex2 = _tex2.getTexID();
    GLuint tex1 = _tex1.getTexID();
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //变换矩阵
        glm::mat4 trans;
        trans=glm::rotate(trans,(GLfloat)glfwGetTime() * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        trans=glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        //变换
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.programID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        //绑定纹理1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);
        //绑定纹理2
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        //

    }
    glfwTerminate();
    return;
}
//练习2：生成另一个盒子。只用平移把这个盒子移到左上角。然后让它随着时间缩放。
void exercise2(){

    //创建窗口
    GLFWwindow *window=initWindow("Transformation-EX2", 800, 600);
    showEnviroment();
    //创建Shader
    Shader shaderProgram("Resources/Shaders/Transformations/shader.vs","Resources/Shaders/Transformations/shader.frag");
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
    Texture _tex2("Resources/Textures/face.png", GL_BGRA, GL_RGBA);
    Texture _tex1("Resources/Textures/container.jpg", GL_BGR, GL_RGB);
    GLuint tex2 = _tex2.getTexID();
    GLuint tex1 = _tex1.getTexID();
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //变换矩阵
        glm::mat4 trans;
        trans=glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans=glm::rotate(trans,(GLfloat)glfwGetTime() * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        //变换
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.programID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        //绑定纹理1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);
        //绑定纹理2
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        //Element1
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        //Element2
        glm::mat4 trans2;
        trans2=glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scale_ratio=sin(glfwGetTime());
        trans2=glm::scale(trans2,glm::vec3(scale_ratio,scale_ratio,scale_ratio));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.programID, "transform"), 1, GL_FALSE, glm::value_ptr(trans2));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);

    }
    //注销显存资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return;
}

};      //  namespace Transformations
#endif  //  TRANSFORMATIONS
