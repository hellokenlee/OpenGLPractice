/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef DEFERRED_SHADING_HPP
#define DEFERRED_SHADING_HPP

///!TODO: 实现Light Volume
namespace DeferredShading{

// 场景内光源的数量
const unsigned int NR_LIGHTS = 128;

GLfloat screenVertices[6*5]={
    // Positions         // TexCoords
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

// Back to Front
//  0  1  2
//  3  4  5
//  6  7  8
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

// deferred shading 实现 // NR_LIGHTS = 128, GT650M 下19ms/frame
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
    cout<<"Lights Num: "<<NR_LIGHTS<<endl;
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
        // 检查是否完整
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            cout<<"Framebuffer not complete!"<<endl;
            exit(-1);
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 随机产生光源
    vector<glm::vec3> lightPositions;
    vector<glm::vec3> lightColors;
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++){
        // calculate slightly random offsets
        float xPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        float yPos = ((rand() % 100) / 100.0) * 10.0 - 6.0;
        float zPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
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
    Shader lightShader("shaders/DeferredShading/light.vert", "shaders/DeferredShading/light.frag");
    // 屏幕对象，一个满屏幕的四边形
    Object screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    screen.setShader(&screenShader);
    // 纳米人模型
    Model nanoMan((GLchar*)"textures/nanosuit/nanosuit.obj");
    nanoMan.setShader(&objShader);
    nanoMan.setCamera(cam);
    // 光源模型(一个球)
    Object *light = Geometry::icoSphere(2);
    light->setShader(&lightShader);
    light->setCamera(cam);
    //
    const float attenConstant = 1.0;
    const float attenLinear = 0.7;
    const float attenQuadratic = 1.8;
    // uniform名字字符串
    char uniformName[128];
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        // Deferred Shaidng: Geometry Pass(第一次pass)
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i = 0; i < 9; ++i){
            nanoMan.modelMat = glm::mat4();
            nanoMan.modelMat = glm::translate(nanoMan.modelMat, objPositions[i]);
            nanoMan.modelMat = glm::scale(nanoMan.modelMat, glm::vec3(0.25f));
            nanoMan.draw();
        }
        // Deferred Shaidng: lighting Pass(第二次pass)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screenShader.use();
        // 写入gBuffer中的纹理
        glUniform1i(glGetUniformLocation(screenShader.programID, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(screenShader.programID, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(screenShader.programID, "gAlbedoSpec"), 2);
        // 写入衰减相关Uniforms
        glUniform1f(glGetUniformLocation(screenShader.programID, "attenConstant"), attenConstant);
        glUniform1f(glGetUniformLocation(screenShader.programID, "attenLinear"), attenLinear);
        glUniform1f(glGetUniformLocation(screenShader.programID, "attenQuadratic"), attenQuadratic);
        for(unsigned int i = 0; i < NR_LIGHTS; ++i){
            sprintf(uniformName, "lights[%d].position", i);
            glUniform3f(glGetUniformLocation(screenShader.programID, uniformName), lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
            sprintf(uniformName, "lights[%d].color", i);
            glUniform3f(glGetUniformLocation(screenShader.programID, uniformName), lightColors[i].x, lightColors[i].y, lightColors[i].z);
            //计算光照半径
            GLfloat lightMax = max(max(lightColors[i].x, lightColors[i].y), lightColors[i].z);
            GLfloat radius = (-attenLinear +  sqrt(attenLinear * attenLinear - 4.0 * attenQuadratic * (attenConstant - (256.0 / 5.0) * lightMax)))/ (2.0 * attenQuadratic);
            sprintf(uniformName, "lights[%d].radius", i);
            glUniform1f(glGetUniformLocation(screenShader.programID, uniformName), radius);
        }
        glUniform3f(glGetUniformLocation(screenShader.programID, "viewPos"), cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
        screen.draw();
        // Forward Shading: 绘制点光源
        // 把原场景的深度缓冲 放入默认FB
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        lightShader.use();
        light->scaleTo(0.04);
        for(unsigned int i = 0; i < NR_LIGHTS; ++i){
            light->moveTo(lightPositions[i]);
            glUniform3f(glGetUniformLocation(lightShader.programID, "objColor"), lightColors[i].x, lightColors[i].y, lightColors[i].z);
            light->draw();
        }
        // 双缓冲交换
        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// forward shading 对比测试 // NR_LIGHTS = 128, GT650M 下 27 ~ 37ms/frame
void exercise1(){
    // 环境初始化
    GLFWwindow *window = initWindow("ForwardShading", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    CameraController::camera.moveto(glm::vec3(0.0f, 0.0f, 8.0f));
    glEnable(GL_DEPTH_TEST);
    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;
    FPSCounter fc;
    //
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
    Shader objShader("shaders/DeferredShading/object.vert", "shaders/DeferredShading/object_forward.frag");
    //
    Model nanoMan((GLchar *)"textures/nanosuit/nanosuit.obj");
    nanoMan.setShader(&objShader);
    nanoMan.setCamera(cam);

    char uniformName[128];
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 写入Uniforms
        objShader.use();
        for(unsigned int i = 0; i < NR_LIGHTS; ++i){
            sprintf(uniformName, "lights[%d].position", i);
            glUniform3f(glGetUniformLocation(objShader.programID, uniformName), lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
            sprintf(uniformName, "lights[%d].color", i);
            glUniform3f(glGetUniformLocation(objShader.programID, uniformName), lightColors[i].x, lightColors[i].y, lightColors[i].z);
        }
        glUniform3f(glGetUniformLocation(objShader.programID, "viewPos"), cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
        // 绘制9个model
        for(int i = 0; i < 9; ++i){
            nanoMan.modelMat = glm::mat4();
            nanoMan.modelMat = glm::translate(nanoMan.modelMat, objPositions[i]);
            nanoMan.modelMat = glm::scale(nanoMan.modelMat, glm::vec3(0.25f));
            nanoMan.draw();
        }
        // 双缓冲
        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// deferred shading 对比测试 // NR_LIGHTS = 128, GT650M 下19ms/frame
void exercise2(){
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
    Shader screenShader("shaders/DeferredShading/screen.vert", "shaders/DeferredShading/screen_deferred.frag");
    Shader lightShader("shaders/DeferredShading/light.vert", "shaders/DeferredShading/light.frag");
    //
    Object screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    screen.setShader(&screenShader);

    Model nanoMan((GLchar *)"textures/nanosuit/nanosuit.obj");
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
        for(unsigned int i = 0; i < NR_LIGHTS; ++i){
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
};
#endif
