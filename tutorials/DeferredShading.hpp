/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef DEFERRED_SHADING_HPP
#define DEFERRED_SHADING_HPP
namespace DeferredShading{

GLfloat screenVertices[6*5]={
    // Positions         // TexCoords
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};


glm::vec3 objPositions[9] = {
    glm::vec3(-3.0,  -3.0, -3.0),
    glm::vec3( 0.0,  -3.0, -3.0),
    glm::vec3( 3.0,  -3.0, -3.0),
    glm::vec3(-3.0,  -3.0,  0.0),
    glm::vec3( 0.0,  -3.0,  0.0),
    glm::vec3( 3.0,  -3.0,  0.0),
    glm::vec3(-3.0,  -3.0,  3.0),
    glm::vec3( 0.0,  -3.0,  3.0),
    glm::vec3( 3.0,  -3.0,  3.0)
};

// deferred shading 实现
void tutorial(){
    // 环境初始化
    GLFWwindow *window = initWindow("DeferredShading", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    CameraController::camera.moveto(glm::vec3(0.0f, 0.0f, 8.0f));
    glEnable(GL_DEPTH_TEST);
    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;
    FPSCounter fc;
    // 准备G缓冲
    GLuint gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        GLuint gPosition, gNormal, gAlbedoSpec;// 片段位置， 法向量， 漫反射颜色信息
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
        glGenTextures(1, &gAlbedoSpec);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
        // 深度缓冲
        GLuint gDepth;
        glGenRenderbuffers(1, &gDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepth);
        // MRT绑定多个输出纹理
        GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, attachments);
        // 检查
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            cout<<"Framebuffer not complete!"<<endl;
            exit(-1);
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //
    const unsigned int NR_LIGHTS = 32;
    vector<glm::vec3> lightPositions;
    vector<glm::vec3> lightColors;
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
        float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // also calculate random color
        float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }
    // 着色器们
    Shader objShader("shaders/DeferredShading/object.vert", "shaders/DeferredShading/object.frag");
    Shader screenShader("shaders/DeferredShading/screen.vert", "shaders/DeferredShading/screen.frag");
    //
    Object screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    screen.setShader(&screenShader);
    Model nanoMan("textures/nanosuit/nanosuit.obj");
    nanoMan.setShader(&objShader);
    nanoMan.setCamera(cam);

    char uniformName[128];
    //
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        // Geometry Pass
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i = 0; i < 9; ++i){
            nanoMan.modelMat = glm::mat4();
            nanoMan.modelMat = glm::translate(nanoMan.modelMat, objPositions[i]);
            nanoMan.modelMat = glm::scale(nanoMan.modelMat, glm::vec3(0.25f));
            nanoMan.draw();
        }
        // LightLing Pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screenShader.use();
        glUniform1i(glGetUniformLocation(screenShader.programID, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(screenShader.programID, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(screenShader.programID, "gAlbedoSpec"), 2);
        for(int i = 0; i < NR_LIGHTS; ++i){
            sprintf(uniformName, "lights[%d].position", i);
            glUniform3f(glGetUniformLocation(screenShader.programID, uniformName), lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
            sprintf(uniformName, "lights[%d].color", i);
            glUniform3f(glGetUniformLocation(screenShader.programID, uniformName), lightColors[i].x, lightColors[i].y, lightColors[i].z);
        }
        glUniform3f(glGetUniformLocation(screenShader.programID, "viewPos"), cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
        screen.draw();
        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// forward shading 对比测试
void exercise1(){

}

};
#endif
