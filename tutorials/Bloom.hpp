/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef BLOOM_HPP
#define BLOOM_HPP
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
    GLFWwindow *window = initWindow("Bloom", 800, 600, keyCallback, CameraController::mouseCallback);
    showEnviroment();
    glfwSwapInterval(0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;
    FPSCounter fc;
    // init Shaders
    Shader cubeShader("shaders/Bloom/cube.vs", "shaders/Bloom/cube.frag");
    Shader lightShader("shaders/Bloom/light.vs", "shaders/Bloom/light.frag");
    Shader screenShader("shaders/Bloom/screen.vs", "shaders/Bloom/screen.frag");
    Shader blurShader("shaders/Bloom/blur.vs", "shaders/Bloom/blur.frag");
    // init Objects
    Object cubeMother(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    cubeMother.setCamera(cam);
    cubeMother.setShader(&cubeShader);
    // set cube models matrices
    Object *cubes[7];
    for(int i = 0; i < 7; ++i){
        cubes[i] = cubeMother.clone();
    }
    cubes[0]->model =  glm::translate(cubes[0]->model, glm::vec3(0.0f, -1.0f, 0.0f));
    cubes[0]->model = glm::scale(cubes[0]->model, glm::vec3(25.0f, 1.0f, 25.0f));
    cubes[1]->model = glm::translate(cubes[1]->model, glm::vec3(0.0f, 1.5f, 0.0f));
    cubes[2]->model = glm::translate(cubes[2]->model, glm::vec3(2.0f, 0.0f, 1.0f));
    cubes[3]->model = glm::translate(cubes[3]->model, glm::vec3(-1.0f, -1.0f, 2.0f));
    cubes[3]->model = glm::rotate(cubes[3]->model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    cubes[3]->model = glm::scale(cubes[3]->model, glm::vec3(2.0f, 2.0f, 2.0f));
    cubes[4]->model = glm::translate(cubes[4]->model, glm::vec3(0.0f, 2.7f, 4.0f));
    cubes[4]->model = glm::rotate(cubes[4]->model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
    cubes[4]->model = glm::scale(cubes[4]->model, glm::vec3(2.5f, 2.5f, 2.5f));
    cubes[5]->model = glm::translate(cubes[5]->model, glm::vec3(-2.0f, 1.0f, -3.0));
    cubes[5]->model = glm::rotate(cubes[5]->model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
    cubes[5]->model = glm::scale(cubes[5]->model, glm::vec3(2.0f, 2.0f, 2.0f));
    cubes[6]->model = glm::translate(cubes[6]->model, glm::vec3(-3.0f, 0.0f, 0.0f));
    // set light's position
    Object *lights[4];
    for(int i = 0; i < 4; ++i){
        lights[i] = cubeMother.clone();
        lights[i]->moveTo(lightPositions[i]);
        lights[i]->setShader(&lightShader);
        lights[i]->scaleTo(0.5f);
    }
    // screen object
    Object screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
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
    // texture
    TextureManager *tm = TextureManager::getManager();
    tm->loadTexture("textures/container2.png", 0, GL_BGRA, GL_SRGB);
    tm->loadTexture("textures/wood.png", 1, GL_BGRA, GL_SRGB);
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
            //render cubes
        cubeShader.use();
        glUniform3fv(glGetUniformLocation(cubeShader.programID, "fViewPosition"), 1, glm::value_ptr(cam->cameraPos));
        for(int i = 0; i < 4; ++i){
            sprintf(uniformNameBuffer, "lightPositions[%d]", i);
            glUniform3fv(glGetUniformLocation(cubeShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lightColors[%d]", i);
            glUniform3fv(glGetUniformLocation(cubeShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightColors[i]));
        }
        tm->bindTexture(1);
        cubes[0]->draw();
        tm->bindTexture(0);
        for(int i = 1; i < 7; ++i){
            cubes[i]->draw();
        }
            //render lights
        for(int i = 0; i < 4; ++i){
            lightShader.use();
            glUniform3fv(glGetUniformLocation(lightShader.programID, "lightColor"), 1, glm::value_ptr(lightColors[i]));
            lights[i]->draw();
        }
        // 做两次连续渲染，高斯模糊
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[0]);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
        blurShader.use();
        glUniform1i(glGetUniformLocation(blurShader.programID, "horizontal"), 1);
        screen.draw();
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[1]);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, pingpingColorBuffers[0]);
        blurShader.use();
        glUniform1i(glGetUniformLocation(blurShader.programID, "horizontal"), 0);
        screen.draw();
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
        //fc.update();
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
