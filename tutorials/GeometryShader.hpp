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
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, //
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, //
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, //
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  //
};

// 教程：画4个点->出现四个房子
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
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 在几何着色器中把顶点向法向量方向平移，表现爆炸效果
void exercise1(){
    GLFWwindow *window=initWindow("GeometryShaderEx1",800,600);
    showEnviroment();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/GeometryShader/explosion.vs","shaders/GeometryShader/explosion.frag");
    shader.addOptionalShader("shaders/GeometryShader/explosion.geom", GL_GEOMETRY_SHADER);

    Model nanosuit("textures/nanosuit/nanosuit.obj");
    nanosuit.setShader(&shader);
    nanosuit.setCamera(&CameraController::camera);

    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glUniform1f(glGetUniformLocation(shader.programID, "time"), glfwGetTime());
        nanosuit.draw();
        ca.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 可视化法向量
void exercise2(){
    GLFWwindow *window=initWindow("GeometryShaderEx1",800,600);
    showEnviroment();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Shader shader("shaders/GeometryShader/showNormals.vs","shaders/GeometryShader/showNormals.frag");
    shader.addOptionalShader("shaders/GeometryShader/showNormals.geom", GL_GEOMETRY_SHADER);

    Shader shader2("shaders/GeometryShader/nanosuit.vs","shaders/GeometryShader/nanosuit.frag");

    Model nanosuit("textures/nanosuit/nanosuit.obj");
    nanosuit.setShader(nullptr);
    nanosuit.setCamera(&CameraController::camera);

    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        nanosuit.setShader(&shader2);
        nanosuit.draw();

        nanosuit.setShader(&shader);
        nanosuit.draw();

        ca.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif  //  GEOMETRY_SHADER_HPP
