/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef BLOOM_CPP
#define BLOOM_CPP

namespace Bloom{

//顶点数据前置声明
extern GLfloat cubeVertices[6*6*8];
extern GLfloat screenVertices[6*5];
// 光源位置
glm::vec3 lightPositions[] = {glm::vec3(0.0f, 0.5f, 1.5f), glm::vec3(-4.0f, 0.5f, -3.0f), glm::vec3(3.0f, 0.5f, 1.0f), glm::vec3(-.8f, 2.4f, -1.0f)};
// 光源颜色
glm::vec3 lightColors[] = {glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(1.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, 1.5f, 0.0f),};
// 重写按键回调
GLfloat exposure = 0.8;
bool bloom = true;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    CameraController::keyCallback(window, key, scancode, action, mode);
    if(key == GLFW_KEY_B && action == GLFW_PRESS){
        //开启/关闭 色调映射
        bloom = !bloom;
        cout<<"Bloom : "<<(bloom ? "ON" : "OFF")<<endl;
    }
    if(key == GLFW_KEY_UP && action == GLFW_PRESS){
        exposure += 0.05;
        cout<<"Exposure: "<<exposure<<endl;
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        exposure -= 0.05;
        cout<<"Exposure: "<<exposure<<endl;
    }
}

// 实现泛光
void tutorial(){
    GLFWwindow *window = initWindow("Bloom", 800, 600);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, CameraController::mouseCallback);
    showEnviroment();
    glfwSwapInterval(0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    CoordinateAxes ca(pCamera);

    // init Shaders
    Shader cubeShader("Resources/Shaders/Bloom/cube.vert", "Resources/Shaders/Bloom/cube.frag");
    Shader lightShader("Resources/Shaders/Bloom/light.vert", "Resources/Shaders/Bloom/light.frag");
    Shader screenShader("Resources/Shaders/Bloom/screen.vert", "Resources/Shaders/Bloom/screen.frag");
    Shader blurShader("Resources/Shaders/Bloom/blur.vert", "Resources/Shaders/Bloom/blur.frag");
    // init Shapes
    std::vector<Shape> cubes;
    for(int i = 0; i < 7; ++i){
        cubes.push_back(Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES));
    }
    cubes[0].setModelMat(glm::translate(cubes[0].getModelMat(), glm::vec3(0.0f, -1.0f, 0.0f)));
    cubes[0].setModelMat(glm::scale(cubes[0].getModelMat(), glm::vec3(25.0f, 1.0f, 25.0f)));
    cubes[1].setModelMat(glm::translate(cubes[1].getModelMat(), glm::vec3(0.0f, 1.5f, 0.0f)));
    cubes[2].setModelMat(glm::translate(cubes[2].getModelMat(), glm::vec3(2.0f, 0.0f, 1.0f)));
    cubes[3].setModelMat(glm::translate(cubes[3].getModelMat(), glm::vec3(-1.0f, -1.0f, 2.0f)));
    cubes[3].setModelMat(glm::rotate(cubes[3].getModelMat(), glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))));
    cubes[3].setModelMat(glm::scale(cubes[3].getModelMat(), glm::vec3(2.0f, 2.0f, 2.0f)));
    cubes[4].setModelMat(glm::translate(cubes[4].getModelMat(), glm::vec3(0.0f, 2.7f, 4.0f)));
    cubes[4].setModelMat(glm::rotate(cubes[4].getModelMat(), glm::radians(23.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))));
    cubes[4].setModelMat(glm::scale(cubes[4].getModelMat(), glm::vec3(2.5f, 2.5f, 2.5f)));
    cubes[5].setModelMat(glm::translate(cubes[5].getModelMat(), glm::vec3(-2.0f, 1.0f, -3.0)));
    cubes[5].setModelMat(glm::rotate(cubes[5].getModelMat(), glm::radians(124.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))));
    cubes[5].setModelMat(glm::scale(cubes[5].getModelMat(), glm::vec3(2.0f, 2.0f, 2.0f)));
    cubes[6].setModelMat(glm::translate(cubes[6].getModelMat(), glm::vec3(-3.0f, 0.0f, 0.0f)));
    // set light's position
    std::vector<Shape> lights;
    for(int i = 0; i < 4; ++i){
        lights.push_back(Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES));
        lights[i].moveTo(lightPositions[i]);
        lights[i].scaleTo(0.5f);
    }
    // screen object
    Shape screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    // FrameBuffers
    GLuint hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        GLuint colorBuffers[2];
        glGenTextures(2, colorBuffers);
        for(int i = 0; i < 2; ++i){
            glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
        }
        GLuint hdrRBO;
        glGenRenderbuffers(1, &hdrRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, hdrRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRBO);
        GLuint attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 高斯模糊的FrameBuffers
    GLuint pingpongFBOs[2];
    GLuint pingpingColorBuffers[2];
    glGenFramebuffers(2, pingpongFBOs);
    glGenTextures(2, pingpingColorBuffers);
    for(int i = 0; i < 2; ++i){
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[i]);
        glBindTexture(GL_TEXTURE_2D, pingpingColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpingColorBuffers[i], 0);
    }
    // textures
    Texture tex0("Resources/Textures/container2.png", GL_BGRA, GL_SRGB);
    Texture tex1("Resources/Textures/wood.png", GL_BGRA, GL_SRGB);
    //
    char uniformNameBuffer[64];
    // Main Loop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        // 第一次渲染
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render cubes
        cubeShader.use();
        glUniform3fv(glGetUniformLocation(cubeShader.programID, "fViewPosition"),
                     1, glm::value_ptr(pCamera->cameraPos));
        for(int i = 0; i < 4; ++i){
            sprintf(uniformNameBuffer, "lightPositions[%d]", i);
            glUniform3fv(glGetUniformLocation(cubeShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lightColors[%d]", i);
            glUniform3fv(glGetUniformLocation(cubeShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightColors[i]));
        }
        tex1.use();
        cubes[0].draw(&cubeShader, pCamera);
        tex0.use();
        for(int i = 1; i < 7; ++i){
            cubes[i].draw(&cubeShader, pCamera);
        }
        // render lights
        for(int i = 0; i < 4; ++i){
            lightShader.use();
            glUniform3fv(glGetUniformLocation(lightShader.programID, "lightColor"), 1, glm::value_ptr(lightColors[i]));
            lights[i].draw(&lightShader, pCamera);
        }
        // 做两次连续渲染，高斯模糊
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[0]);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
        blurShader.use();
        glUniform1i(glGetUniformLocation(blurShader.programID, "horizontal"), 1);
        screen.draw(&blurShader, pCamera);
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[1]);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, pingpingColorBuffers[0]);
        blurShader.use();
        glUniform1i(glGetUniformLocation(blurShader.programID, "horizontal"), 0);
        screen.draw(&blurShader, pCamera);
        // 把模糊的和非模糊的加起来，形成泛光效果
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpingColorBuffers[1]);
        screenShader.use();
        glUniform1i(glGetUniformLocation(screenShader.programID, "image"), 0);
        glUniform1i(glGetUniformLocation(screenShader.programID, "image_blur"), 1);
        glUniform1f(glGetUniformLocation(screenShader.programID, "exposure"), exposure);
        glUniform1i(glGetUniformLocation(screenShader.programID, "bloom"), bloom);
        screen.draw();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glfwSwapBuffers(window);
        //
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLfloat screenVertices[6*5]={
    // Positions         // TexCoords
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};
GLfloat cubeVertices[6*6*8] = {
    // Back face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
    // Front face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
    // Left face
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
    // Right face
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
    // Bottom face
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    // Top face
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
};

};
#endif
