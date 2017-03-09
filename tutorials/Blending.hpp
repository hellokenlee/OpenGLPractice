/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef BLENDING_HPP
#define BLENDING_HPP
namespace Blending{
extern GLfloat squareVertices[5*6];
extern GLfloat planeVertices[5*6];
extern GLfloat cubeVertices[36*5];
extern glm::vec3 grassPositions[5];
extern glm::vec3 cubePositions[2];
void tutorial(){
    //
    GLFWwindow* window=initWindow("Blending",800,600);
    showEnviroment();
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    //
    CoordinateAxes ca(&CameraController::camera);
    //草
    Shader grassShader("shaders/Blending/scene.vs","shaders/Blending/discard.frag");
    Object grassSquare(squareVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    grassSquare.setShader(&grassShader);
    grassSquare.setCamera(&CameraController::camera);
    grassSquare.moveTo(glm::vec3(0,0.5,0));
    //地面
    Object ground(planeVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    ground.setShader(&grassShader);
    ground.setCamera(&CameraController::camera);
    //箱子
    Object box(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    box.setShader(&grassShader);
    box.setCamera(&CameraController::camera);
    //纹理
    TextureManager *tm=TextureManager::getManager();
    tm->loadTexture("textures/grass.png",0,GL_BGRA,GL_RGBA,0,0,GL_CLAMP_TO_EDGE);
    tm->loadTexture("textures/timg2.jpg",1,GL_BGR,GL_RGB);
    tm->loadTexture("textures/container2.png",2,GL_BGRA,GL_RGBA);
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        CameraController::update();

        //ca.draw();
        //画草
        tm->bindTexture(0);
        for(auto pos:grassPositions){
            grassSquare.moveTo(pos);
            grassSquare.draw();
        }
        //画地面
        tm->bindTexture(1);
        ground.draw();
        //画箱子
        tm->bindTexture(2);
        for(auto pos:cubePositions){
            box.moveTo(pos);
            box.draw();
        }

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool cmp(glm::vec3 pos1,glm::vec3 pos2){
    GLfloat dist1=glm::length(pos1-CameraController::camera.cameraPos);
    GLfloat dist2=glm::length(pos2-CameraController::camera.cameraPos);
    return dist1>dist2;
}

void exercise1(){
    //
    GLFWwindow* window=initWindow("Blending",800,600);
    showEnviroment();
    glfwSwapInterval(0);
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //
    CoordinateAxes ca(&CameraController::camera);
    //半透明玻璃
    Shader grassShader("shaders/Blending/scene.vs","shaders/Blending/blend.frag");
    Object grassSquare(squareVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    grassSquare.setShader(&grassShader);
    grassSquare.setCamera(&CameraController::camera);
    grassSquare.moveTo(glm::vec3(0,0.5,0));
    map<GLfloat,glm::vec3> sortedGrass;
    for(auto pos:grassPositions){
        GLfloat dist=glm::length((CameraController::camera).cameraPos-pos);
        sortedGrass[dist]=pos;
    }
    //地面
    Object ground(planeVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    ground.setShader(&grassShader);
    ground.setCamera(&CameraController::camera);
    //箱子
    Object box(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    box.setShader(&grassShader);
    box.setCamera(&CameraController::camera);
    //纹理
    TextureManager *tm=TextureManager::getManager();
    tm->loadTexture("textures/glass.png",0,GL_BGRA,GL_RGBA,0,0,GL_CLAMP_TO_EDGE);
    tm->loadTexture("textures/timg2.jpg",1,GL_BGR,GL_RGB);
    tm->loadTexture("textures/container2.png",2,GL_BGRA,GL_RGBA);
    //
    FPSCounter fp;
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        CameraController::update();

        //ca.draw();
        //画地面
        tm->bindTexture(1);
        ground.draw();
         //画箱子
        tm->bindTexture(2);
        for(auto pos:cubePositions){
            box.moveTo(pos);
            box.draw();
        }
        //画玻璃，透明的永远要最后画

        tm->bindTexture(0);
         /*
        for(auto it=sortedGrass.rbegin();it!=sortedGrass.rend();++it){
            grassSquare.moveTo(it->second);
            grassSquare.draw();
        }
        */
        sort(grassPositions,grassPositions+5,cmp);
        for(auto pos:grassPositions){
            grassSquare.moveTo(pos);
            grassSquare.draw();
        }

        glfwSwapBuffers(window);
        fp.update();
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
    0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  1.0f
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
