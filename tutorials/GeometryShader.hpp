/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef GEOMETRY_SHADER_CPP
#define GEOMETRY_SHADER_CPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

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
    GLFWwindow *window = initWindow("GeometryShader",800,600);
    showEnviroment();

    Shader shader("Resources/Shaders/GeometryShader/pointAsHouse.vs","Resources/Shaders/GeometryShader/pointAsHouse.frag");
    shader.addOptionalShader("Resources/Shaders/GeometryShader/pointAsHouse.geom", GL_GEOMETRY_SHADER);

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
    CameraController::bindControl(window);
    glEnable(GL_DEPTH_TEST);
    //
    Shader shader("Resources/Shaders/GeometryShader/explosion.vs","Resources/Shaders/GeometryShader/explosion.frag");
    shader.addOptionalShader("Resources/Shaders/GeometryShader/explosion.geom", GL_GEOMETRY_SHADER);
    //
    Model nanosuit((GLchar*)"Resources/Meshes/nanosuit/nanosuit.obj");
    //
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        shader.use();
        glUniform1f(glGetUniformLocation(shader.programID, "time"), glfwGetTime());
        nanosuit.draw(&shader, CameraController::getCamera());
        //
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

std::vector<float> vertices2 = {
    // POSITION_XYZ         NORMAL_XYZ          TEXTURE_UV
    //前面
    -1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
     1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    // 后面
    -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,   0.0f, 0.0f,-1.0f,   1.0f, 0.0f,
    // 上面
    -1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    // 下面
    -1.0f, -1.0f, -1.0f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f,
     1.0f, -1.0f,  1.0f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,   0.0f,-1.0f, 0.0f,   1.0f, 0.0f,
    // 左边
    -1.0f, -1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    // 右边
     1.0f, -1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
     1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f
};
std::vector<unsigned int> indices2 = {
    // Front Face
    0,  2,  1,
    0,  3,  2,
    // Back Face
    4,  6,  5,
    4,  7,  6,
    // Top Face
    8, 10, 9,
    8, 11, 10,
    // Bottom Face
    12, 14, 13,
    12, 15, 14,
    // Left Face
    16, 18, 17,
    16, 19, 18,
    // Right Face
    20, 22, 21,
    20, 23, 22
};
// 可视化法向量
void exercise2(){
    GLFWwindow *window=initWindow("GeometryShaderEx1",800,600);
    showEnviroment();
    Camera* pCamera = CameraController::getCamera();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Shader shader1("Resources/Shaders/GeometryShader/showNormals.vs","Resources/Shaders/GeometryShader/showNormals.frag");
    shader1.addOptionalShader("Resources/Shaders/GeometryShader/showNormals.geom", GL_GEOMETRY_SHADER);

    Shader shader2("Resources/Shaders/GeometryShader/nanosuit.vs","Resources/Shaders/GeometryShader/nanosuit.frag");

    Model nanosuit((GLchar*)"Resources/Meshes/nanosuit/nanosuit.obj");

    Shape cube(vertices2.data(), vertices2.size() / 8, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES, indices2.data(), indices2.size());
    cube.moveTo(glm::vec3(0.0, 0.0, 5.0));
    //显示坐标轴
    CoordinateAxes ca(pCamera);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        nanosuit.draw(&shader2, pCamera);
        nanosuit.draw(&shader1, pCamera);
        //
        cube.draw();

        ca.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif  //  GEOMETRY_SHADER_HPP
