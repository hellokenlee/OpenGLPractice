/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef BLENDING_CPP
#define BLENDING_CPP

#include <map>

namespace Blending{

extern GLfloat squareVertices[5*6];
extern GLfloat planeVertices[5*6];
extern GLfloat cubeVertices[36*5];
extern glm::vec3 grassPositions[5];
extern glm::vec3 cubePositions[2];

// 绘制透明物体
void tutorial(){
    //
    GLFWwindow* window=initWindow("Blending",800,600);
    showEnviroment();
    // Set the required callback functions
    CameraController::bindControl(window);
    Camera* pCamera = CameraController::getCamera();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    //
    CoordinateAxes ca(pCamera);
    // 草
    Shader shader("Resources/Shaders/Blending/scene.vs","Resources/Shaders/Blending/discard.frag");
    Shape grassSquare(squareVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    grassSquare.moveTo(glm::vec3(0.0f, 0.5f, 0.0f));
    // 地面
    Shape ground(planeVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    //箱子
    Shape box(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    //纹理
    //Texture tex0("Resources/Textures/glass.png", GL_BGRA, GL_RGBA, 0, 0, GL_CLAMP_TO_EDGE); //启用这个显示半透明物体有渲染bug
    Texture tex0("Resources/Textures/grass.png", GL_BGRA, GL_RGBA, 0, 0, GL_CLAMP_TO_EDGE);
    Texture tex1("Resources/Textures/timg2.jpg", GL_BGR,GL_RGB);
    Texture tex2("Resources/Textures/container2.png", GL_BGRA, GL_RGBA);
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        CameraController::update();

        //画草
        tex0.use(0);
        for(auto pos:grassPositions){
            grassSquare.moveTo(pos);
            grassSquare.draw(&shader, pCamera);
        }
        //画地面
        tex1.use(0);
        ground.draw(&shader, pCamera);
        //画箱子
        tex2.use(0);
        for(auto pos:cubePositions){
            box.moveTo(pos);
            box.draw(&shader, pCamera);
        }

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool cmp(glm::vec3 pos1,glm::vec3 pos2){
    GLfloat dist1 = glm::length(pos1 - CameraController::getCamera()->cameraPos);
    GLfloat dist2 = glm::length(pos2 - CameraController::getCamera()->cameraPos);
    return dist1 > dist2;
}

// 排序绘制半透明玻璃
void exercise1(){
    //
    GLFWwindow* window = initWindow("Blending", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    // Set the required callback functions
    CameraController::bindControl(window);
    Camera* pCamera = CameraController::getCamera();
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //
    CoordinateAxes ca(pCamera);
    //半透明玻璃
    Shader shader("Resources/Shaders/Blending/scene.vs","Resources/Shaders/Blending/blend.frag");
    //
    Shape grassSquare(squareVertices, 6, POSITIONS_TEXTURES,GL_TRIANGLES);
    grassSquare.moveTo(glm::vec3(0,0.5,0));
    std::map<GLfloat,glm::vec3> sortedGrass;
    for(auto pos:grassPositions){
        GLfloat dist = glm::length(pCamera->cameraPos - pos);
        sortedGrass[dist] = pos;
    }
    //地面
    Shape ground(planeVertices, 6, POSITIONS_TEXTURES,GL_TRIANGLES);
    //箱子
    Shape box(cubeVertices, 36, POSITIONS_TEXTURES,GL_TRIANGLES);
    //纹理
    Texture tex0("Resources/Textures/glass.png", GL_BGRA, GL_RGBA, 0, 0, GL_CLAMP_TO_EDGE);
    Texture tex1("Resources/Textures/timg2.jpg", GL_BGR,GL_RGB);
    Texture tex2("Resources/Textures/container2.png", GL_BGRA, GL_RGBA);
    //
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        CameraController::update();

        // ca.draw();
        // 画地面
        tex1.use();
        ground.draw(&shader, pCamera);
        // 画箱子
        tex2.use();
        for(auto pos:cubePositions){
            box.moveTo(pos);
            box.draw(&shader, pCamera);
        }
        //画玻璃，透明的永远要最后画
        tex0.use();
        sort(grassPositions, grassPositions + 5, cmp);
        for(auto pos : grassPositions){
            grassSquare.moveTo(pos);
            grassSquare.draw(&shader, pCamera);
        }

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
glm::vec3 grassPositions[5]={
    glm::vec3(-1.5f,  0.0f, -0.48f),
    glm::vec3( 1.5f,  0.0f,  0.51f),
    glm::vec3( 0.0f,  0.0f,  0.7f),
    glm::vec3(-0.3f,  0.0f, -2.3f),
    glm::vec3( 0.5f,  0.0f, -0.6f)
};
glm::vec3 cubePositions[2]={
    glm::vec3(-1.0f, 0.0f, -1.0f),
    glm::vec3(2.0f, 0.0f, 0.0f)
};
GLfloat squareVertices[] = {
    // Positions         // Texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};
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
#endif // BLENDING_HPP
