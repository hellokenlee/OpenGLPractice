/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef HDR_CPP
#define HDR_CPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace HDR{

extern GLfloat screenVertices[6*5];
extern GLfloat cubeVertices[6*6*8];
glm::vec3 lightPositions[] = {glm::vec3(0.0f, 0.0f, 49.5f), glm::vec3(-1.4f, -1.9f, 9.0f), glm::vec3(0.0f, -1.8f, 4.0f), glm::vec3(0.8f, -1.7f, 6.0f)};
glm::vec3 lightColors[] = {glm::vec3(200.0f, 200.0f, 200.0f), glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.2f), glm::vec3(0.0f, 0.1f, 0.0f)};
bool doToneMapping = false;
GLfloat exposure = 0.05;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    CameraController::keyCallback(window, key, scancode, action, mode);
    if(key == GLFW_KEY_M && action == GLFW_PRESS){
        //开启/关闭 色调映射
        doToneMapping = !doToneMapping;
        cout<<"Tone Mapping : "<<(doToneMapping ? "ON" : "OFF")<<endl;
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

void tutorial(){
    GLFWwindow *window = initWindow("HDR", 800, 600);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, CameraController::mouseCallback);
    showEnviroment();
    glfwSwapInterval(0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    CoordinateAxes ca(pCamera);
    FPSCounter fc;
    // 生成HDR纹理附件
    GLuint hdrTexture;
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 生成FBO, 附着纹理附件
    GLuint hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTexture, 0);
        // 检查
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "Framebuffer not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 着色器
    Shader tunnelShader("Resources/Shaders/HDR/tunnel.vs", "Resources/Shaders/HDR/tunnel.frag");
    Shader screenShader("Resources/Shaders/HDR/screen.vs", "Resources/Shaders/HDR/screen.frag");
    // 隧道物体
    Shape tunnel(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    tunnel.setModelMat(glm::translate(tunnel.getModelMat(), glm::vec3(0.0, 0.0, 25.0)));
    tunnel.setModelMat(glm::scale(tunnel.getModelMat(), glm::vec3(5.0, 5.0, 55.0)));
    //
    Shape screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    // 纹理
    Texture tex0("Resources/Textures/wood.png", GL_BGRA, GL_SRGB);
    //
    char uniformNameBuffer[64];
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        //
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        tunnelShader.use();
        for(int i = 0; i < 4; ++i){
            sprintf(uniformNameBuffer, "lightPositions[%d]", i);
            glUniform3fv(glGetUniformLocation(tunnelShader.programID, uniformNameBuffer),
                         1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lightColors[%d]", i);
            glUniform3fv(glGetUniformLocation(tunnelShader.programID, uniformNameBuffer),
                         1, glm::value_ptr(lightColors[i]));
        }
        glUniform3fv(glGetUniformLocation(tunnelShader.programID, "fViewPosition"),
                     1, glm::value_ptr(pCamera->cameraPos));
        tex0.use(0);
        tunnel.draw(&tunnelShader, pCamera);
        //
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        screenShader.use();
        glUniform1i(glGetUniformLocation(screenShader.programID, "toLDR"), doToneMapping);
        glUniform1f(glGetUniformLocation(screenShader.programID, "exposure"), exposure);
        screen.draw();
        //
        glfwSwapBuffers(window);
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
#endif // HDR_HPP
