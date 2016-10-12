/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef COORDINATESYSTEM_HPP
#define COORDINATESYSTEM_HPP
namespace CoordinateSystem{
//窗口宽高
const float ScreenHeight=600.0;
const float ScreenWidth=800.0;
//顶点信息前置声明
extern GLfloat vertices[36*5];
//位置信息前置声明
extern glm::vec3 cubePositions[10];
//教程实现：
void tutorial(){
    FPSCounter fc;
    //窗口初始化
    GLFWwindow *window=initWindow("CoordinateSystem",ScreenWidth,ScreenHeight,nullptr);
    //关闭垂直同步
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    showEnviroment();
    //创建Shader
    Shader shaderProgram("shaders/CoordinateSystem/shader.vs","shaders/CoordinateSystem/shader.frag");
    //读入数据
    GLuint VBO,VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=Textures::loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);
    GLuint tex1=Textures::loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_REPEAT,GL_LINEAR);
    //初始化矩阵
    //模型矩阵(局部coord -> 世界coord)
    glm::mat4 model;
    //观察矩阵(世界coord -> 观察coord)
    glm::mat4 view;
    view=glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //投影矩阵(观察coord -> 裁剪coord)
    glm::mat4 projection;
    projection=glm::perspective(45.0f,ScreenWidth/ScreenHeight,0.1f,100.0f);
    //3个矩阵的Shader uniform位置
    GLuint modelPos=glGetUniformLocation(shaderProgram.programID,"model");
    GLuint viewPos=glGetUniformLocation(shaderProgram.programID,"view");
    GLuint projPos=glGetUniformLocation(shaderProgram.programID,"proj");
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //旋转
        //model=glm::mat4();
        //model=glm::rotate(model, (GLfloat)glfwGetTime()*50.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        //设置3个坐标变换矩阵
        //glUniformMatrix4fv(modelPos,1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(viewPos,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projPos,1,GL_FALSE,glm::value_ptr(projection));
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
        for(GLuint i=0;i<10;i++){
          model=glm::mat4();
          model = glm::translate(model, cubePositions[i]);
          GLfloat angle = 20.0f * i*(GLfloat)glfwGetTime();
          model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
          glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));

          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        fc.update();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return;
}
//位置信息
glm::vec3 cubePositions[10] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
//顶点信息
GLfloat vertices[36*5] = {
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
};      //  namespace CoordinateSystem
#endif  //  COORDINATESYSTEM
