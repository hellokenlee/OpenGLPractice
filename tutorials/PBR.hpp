/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef PBR_CPP
#define PBR_CPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace PBR{

// 光源位置
glm::vec3 lightPositions[4] = {
    glm::vec3(-10.0f,  10.0f, 10.0f),
    glm::vec3( 10.0f,  10.0f, 10.0f),
    glm::vec3(-10.0f, -10.0f, 10.0f),
    glm::vec3( 10.0f, -10.0f, 10.0f)
};
// 光源颜色/强度
glm::vec3 lightColors[] = {
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f)
};


// 使用 Cook-Torrance BRDF 渲染， 这章的重要实现都在Shader中， 请注意。
void tutorial(){
    GLFWwindow *window = initWindow("PBR", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //
    CoordinateAxes ca(pCamera);
    FPSCounter fc;
    ControlPanel panel(window);
    //
    Shader pbrShader("Resources/Shaders/PBR/pbr.vs", "Resources/Shaders/PBR/pbr.frag");
    Shader sphereShader("Resources/Shaders/PBR/sphere.vs", "Resources/Shaders/PBR/sphere.frag");
    Shader normalShader("Resources/Shaders/PBR/showNormals.vs", "Resources/Shaders/PBR/showNormals.frag");
    normalShader.addOptionalShader("Resources/Shaders/PBR/showNormals.geom", GL_GEOMETRY_SHADER);
    //
    std::unique_ptr<Shape> sphere = Geometry::createIcoSphere(2);
    char uniformNameBuffer[128];
    int rowNum = 7, colNum = 7;
    //
    while(!glfwWindowShouldClose(window)){
        //
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        pbrShader.use();
        // 写入光源信息
        for(int i = 0; i < 4; ++i){
            sprintf(uniformNameBuffer, "lights[%d].worldPos", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lights[%d].color", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightColors[i]));
        }
        // 写入视角位置
        glUniform3fv(glGetUniformLocation(pbrShader.programID, "viewPos"),
                     1, glm::value_ptr(pCamera->cameraPos));
        // 写入不变的材质信息
        glUniform3f(glGetUniformLocation(pbrShader.programID, "material.albedo"), 0.5f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(pbrShader.programID, "material.ao"), 1.0f);
        // 按位置写入材质，位置信息
        for(int r = 0; r < rowNum; ++r){
            glUniform1f(glGetUniformLocation(pbrShader.programID, "material.metallic"), float(r) / float(rowNum));
            for(int c = 0; c < colNum; ++c){
                glUniform1f(glGetUniformLocation(pbrShader.programID, "material.roughness"),
                            glm::clamp(float(c) / float(colNum), 0.05f, 1.0f));
                sphere->moveTo(glm::vec3(2.5 * (c - ((float)colNum / 2.0)), 2.5 * (r - ((float)rowNum / 2.0)), 0.0f));
                sphere->draw(&pbrShader, pCamera);
            }
        }
        //
        panel.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 纹理PBR， 预计算TBN矩阵，手动传入
/* 缺少预计算TBN.
void exercise1(){
    GLFWwindow *window = initWindow("PBR-Ex1", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //
    CoordinateAxes ca(pCamera);
    FPSCounter fc;
    //
    Shader pbrShader("Resources/Shaders/PBR/pbr_texture3.vs", "Resources/Shaders/PBR/pbr_texture3.frag");
    Shader showTBNShader("Resources/Shaders/PBR/showTBN.vs", "Resources/Shaders/PBR/showTBN.frag");
    showTBNShader.addOptionalShader("Resources/Shaders/PBR/showTBN.geom", GL_GEOMETRY_SHADER);
    //
    std::unique_ptr<Shape> sphere = Geometry::createIcoSphere(1);
    //
    Texture tex0("Resources/Textures/sphere/rustediron2_basecolor.png", GL_BGRA, GL_RGBA);
    Texture tex1("Resources/Textures/sphere/rustediron2_normal.png", GL_BGRA, GL_RGBA);
    Texture tex2("Resources/Textures/sphere/rustediron2_metallic.png", GL_BGRA, GL_RGBA);
    Texture tex3("Resources/Textures/sphere/rustediron2_roughness.png", GL_BGRA, GL_RGBA);
    Texture tex4("Resources/Textures/sphere/rustediron2_ao.png", GL_BGRA, GL_RGBA);
    //
    char uniformNameBuffer[128];
    int rowNum = 7, colNum = 7;
    //
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        pbrShader.use();
        // 写入光源信息
        for(int i = 0; i < 4; ++i){
            sprintf(uniformNameBuffer, "lights[%d].position", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer),
                         1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lights[%d].color", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer),
                         1, glm::value_ptr(lightColors[i]));
        }
        // 写入视角位置
        glUniform3fv(glGetUniformLocation(pbrShader.programID, "viewPos"), 1,
                     glm::value_ptr(pCamera->cameraPos));
        //贴图
        tex0.use(0);
        tex1.use(1);
        tex2.use(2);
        tex3.use(3);
        tex4.use(4);
        // 写入贴图
        glUniform1i(glGetUniformLocation(pbrShader.programID, "albedoMap"), 0);
        glUniform1i(glGetUniformLocation(pbrShader.programID, "normalMap"), 1);
        glUniform1i(glGetUniformLocation(pbrShader.programID, "metallicMap"), 2);
        glUniform1i(glGetUniformLocation(pbrShader.programID, "roughnessMap"), 3);
        glUniform1i(glGetUniformLocation(pbrShader.programID, "aoMap"), 4);
        // 按位置改变model矩阵
        for(int r = 0; r < rowNum; ++r){
            for(int c = 0; c < colNum; ++c){
                sphere->moveTo(glm::vec3(2.5 * (c - ((float)colNum / 2.0)), 2.5 * (r - ((float)rowNum / 2.0)), 0.0f));
                sphere->draw(&pbrShader, pCamera);
            }
        }
        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/
// 纹理PBR， 在片段着色器中计算TBN矩阵
void exercise2(){
    GLFWwindow *window = initWindow("PBR-Ex2", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //
    CoordinateAxes ca(pCamera);
    ControlPanel panel(window);
    //
    Shader pbrShader("Resources/Shaders/PBR/pbr_texture2.vs", "Resources/Shaders/PBR/pbr_texture2.frag");
    std::unique_ptr<Shape> sphere = Geometry::createIcoSphere(4);
    //
    Texture tex0("Resources/Textures/sphere/rustediron2_basecolor.png", GL_BGRA, GL_RGBA);
    Texture tex1("Resources/Textures/sphere/rustediron2_normal.png", GL_BGRA, GL_RGBA);
    Texture tex2("Resources/Textures/sphere/rustediron2_metallic.png", GL_BGRA, GL_RGBA);
    Texture tex3("Resources/Textures/sphere/rustediron2_roughness.png", GL_BGRA, GL_RGBA);
    Texture tex4("Resources/Textures/sphere/rustediron2_ao.png", GL_BGRA, GL_RGBA);
    //
    char uniformNameBuffer[128];
    int rowNum = 7,colNum = 7;
    //
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //ca.draw();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        pbrShader.use();
        // 写入光源信息
        for(int i = 0; i < 4; ++i){
            sprintf(uniformNameBuffer, "lightPositions[%d]", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer),
                         1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lightColors[%d]", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer),
                         1, glm::value_ptr(lightColors[i]));
        }
        // 写入视角位置
        glUniform3fv(glGetUniformLocation(pbrShader.programID, "camPos"),
                     1, glm::value_ptr(pCamera->cameraPos));
        //贴图
        tex0.use(0);
        tex1.use(1);
        tex2.use(2);
        tex3.use(3);
        tex4.use(4);
        // 写入贴图
        glUniform1i(glGetUniformLocation(pbrShader.programID, "albedoMap"), 0);
        glUniform1i(glGetUniformLocation(pbrShader.programID, "normalMap"), 1);
        glUniform1i(glGetUniformLocation(pbrShader.programID, "metallicMap"), 2);
        glUniform1i(glGetUniformLocation(pbrShader.programID, "roughnessMap"), 3);
        glUniform1i(glGetUniformLocation(pbrShader.programID, "aoMap"), 4);
        // 按位置改变model矩阵
        for(int r = 0; r < rowNum; ++r){
            for(int c = 0; c < colNum; ++c){
                sphere->moveTo(glm::vec3(2.5 * (c - ((float)colNum / 2.0)), 2.5 * (r - ((float)rowNum / 2.0)), 0.0f));
                sphere->draw(&pbrShader, pCamera);
            }
        }
        //
        panel.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif // PBR_HPP
