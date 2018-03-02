/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef DEPTH_TEST_CPP
#define DEPTH_TEST_CPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace DepthTest{

//顶点信息前置声明
extern GLfloat cubeVertices[36*5];
extern GLfloat planeVertices[6*5];
//
glm::vec3 cubePositions[2] = {glm::vec3(-1.0f, 0.0f, -1.0f),glm::vec3(2.0f, 0.0f, 0.0f)};

// 可视化深度缓冲以及解决Z-Fighting
void tutorial(){
    // Init GLFW
    GLFWwindow* window = initWindow("DepthTest",800,600);
    // Set the required callback functions
    CameraController::bindControl(window);
    Camera* pCamera = CameraController::getCamera();
    pCamera->moveto(glm::vec3(0.0f, -0.4f, 3.0f));
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
    // Setup and compile our shaders
    //Shader shader("Resources/Shaders/DepthTest/scene1.vs", "Resources/Shaders/DepthTest/scene1.frag");
    Shader shader("Resources/Shaders/DepthTest/scene1.vs", "Resources/Shaders/DepthTest/zVisualize.frag"); // 可视化深度缓冲
    // Setup cube VAO
    Shape cube(cubeVertices, 36, POSITIONS_TEXTURES, GL_TRIANGLES);
    // Setup plane VAO
    Shape plane(planeVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    // Load textures
    Texture tex("Resources/Textures/container.jpg", GL_BGR, GL_RGB);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    //
    tex.use();
    // Game loop
    while(!glfwWindowShouldClose(window)){
        //
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        ca.draw();

        plane.draw(&shader, pCamera);

        cube.moveTo(cubePositions[0]);
        cube.draw(&shader, pCamera);
        cube.moveTo(cubePositions[1]);
        cube.draw(&shader, pCamera);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

// Set the object data (buffers, vertex attributes)
GLfloat cubeVertices[6*6*5] = {
    // Positions          // Texture Coords
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
GLfloat planeVertices[6*5] = {
    // Positions            // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
    5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
};
};
#endif // DEPTH_TEST_HPP
