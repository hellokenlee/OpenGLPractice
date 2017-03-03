/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef STENCIL_TEST_HPP
#define STENCIL_TEST_HPP
namespace StencilTest{

//顶点信息前置声明
extern GLfloat cubeVertices[36*5];
extern GLfloat planeVertices[6*5];

//箱子位置
glm::vec3 cubePositions[2]={glm::vec3(-1.0f, 0.0f, -1.0f),glm::vec3(2.0f, 0.0f, 0.0f)};

void tutorial(){
    // Init GLFW
    GLFWwindow* window=initWindow("DepthTest",800,600);
    showEnviroment();
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // Setup and compile our shaders
    Shader shader("shaders/StencilTest/scene1.vs", "shaders/StencilTest/scene1.frag");
    Shader edgeShader("shaders/StencilTest/scene1.vs", "shaders/StencilTest/shaderSingleColor.frag");
    // Setup cube VAO
    Object cube(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&shader);
    // Setup plane VAO
    Object plane(planeVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    plane.setCamera(&CameraController::camera);
    plane.setShader(&shader);
    // Load textures
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container2.png",0,GL_BGRA,GL_RGBA))
        return ;
    tm->bindTexture(0);
    // Show axies
    CoordinateAxes ca(&CameraController::camera);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        //开启模板缓冲写，不然clear就没作用了。
        glStencilMask(0xFF);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        //关闭模板缓冲写，绘制其他东西的时候保证模板没有被写入
        glStencilMask(0x00);
        glClearColor(0.0f,0.0f,0.0f,1.0f);

        CameraController::update();
        plane.moveTo(glm::vec3(0.0,-0.1,0));
        plane.draw();
        ca.draw();

        //第一轮绘制箱子
        //  设置模板缓冲为可写状态
        glStencilMask(0xFF);
        //  无论绘制了什么值的内容，通过模板测试(1可以为其他任意值，因为是GL_ALWAYS)
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        cube.setShader(&shader);
        cube.scaleTo(1.0);
        cube.moveTo(cubePositions[0]);
        cube.draw();
        cube.moveTo(cubePositions[1]);
        cube.draw();
        //第二轮绘制箱子，绘制出绿色边
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        cube.setShader(&edgeShader);
        cube.scaleTo(1.1);
        cube.moveTo(cubePositions[0]);
        cube.draw();
        cube.moveTo(cubePositions[1]);
        cube.draw();

        glfwSwapBuffers(window);
    }
    glfwTerminate();

}

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
#endif //STENCIL_TEST_HPP
