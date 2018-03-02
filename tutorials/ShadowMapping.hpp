/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef SHADOW_MAPPING_CPP
#define SHADOW_MAPPING_CPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace ShadowMapping{

const int SCR_WIDTH  = 1080;
const int SCR_HEIGHT = 720;

// 顶点信息前置声明
extern GLfloat screenVertices[6*5];
extern GLfloat cubeVertices[6*6*8];
extern GLfloat planeVertices[6*8];
// 立方体位置
glm::vec3 cubePositions[2] = {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.0f, 1.0f)};
// ShadowMap的解析度
const GLuint SHADOW_MAP_WIDTH = 1024;
const GLuint SHADOW_MAP_HEIGHT = 1024;
glm::vec3 lightPosition = glm::vec3(-2.0f, 1.5f, -1.5f);

// 教程实现: 在定向光下物体光照和阴影
void tutorial(){
    GLFWwindow *window = initWindow("ShadowMapping", SCR_WIDTH, SCR_HEIGHT);
    glfwSwapInterval(0);
    showEnviroment();
    Camera* pCamera = CameraController::getCamera();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);
    //
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    CoordinateAxes ca(pCamera);
    // 生成ShadowMap
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 生成ShadowMap的FBO, 绑定纹理
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 平行光的投影矩阵(正交)
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    // 平行光的视角矩阵
    glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    // 计算ShadowMap的着色器
    Shader depthMapShader("Resources/Shaders/ShadowMapping/light.vs", "Resources/Shaders/ShadowMapping/light.frag");
    // 可视化ShadowMap的着色器
    Shader debugShader("Resources/Shaders/ShadowMapping/screen.vs", "Resources/Shaders/ShadowMapping/screen.frag");
    // 普通场景的着色器
    Shader sceneShader("Resources/Shaders/ShadowMapping/shadowed_object_directional_light.vs", "Resources/Shaders/ShadowMapping/shadowed_object_directional_light.frag");
    // 立方体物体
    Shape *cube1 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    Shape *cube2 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    Shape *cube3 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    glm::mat4 tmpModel = glm::mat4();
    tmpModel = glm::translate(tmpModel, glm::vec3(-1.0f, 0.0f, 2.0));
    tmpModel = glm::rotate(tmpModel, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    tmpModel = glm::scale(tmpModel, glm::vec3(0.5));
    cube1->moveTo(cubePositions[0]);
    cube2->moveTo(cubePositions[1]);
    cube3->setModelMat(tmpModel);
    // 地面
    Shape plane(planeVertices, 6, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    // 屏幕
    Shape screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    // 纹理
    Texture tex0("Resources/Textures/container.jpg", GL_BGR, GL_RGB);
    Texture tex1("Resources/Textures/wood.jpg", GL_BGR, GL_RGB);
    FPSCounter fc;
    // MainLoop
    while(!glfwWindowShouldClose(window)){
        //
        glfwPollEvents();
        CameraController::update();
        // 从光源处绘制，生成ShadowMap
        glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
        glCullFace(GL_FRONT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            depthMapShader.use();
            // 光源视角矩阵
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "lightSpaceMatrix"),
                               1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            // 绘制地面
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(plane.getModelMat()));
            plane.draw(&depthMapShader);
            //绘制立方体
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube1->getModelMat()));
            cube1->draw(&depthMapShader);
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube2->getModelMat()));
            cube2->draw(&depthMapShader);
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube3->getModelMat()));
            cube3->draw(&depthMapShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 第二次绘制，根据ShadowMap绘制场景
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sceneShader.use();
        glUniformMatrix4fv(glGetUniformLocation(sceneShader.programID, "lightSpaceMatrix"),
                           1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform3f(glGetUniformLocation(sceneShader.programID, "lightPosition"),
                    lightPosition.x, lightPosition.y, lightPosition.z);
        glUniform3f(glGetUniformLocation(sceneShader.programID, "viewPosition"),
                    pCamera->cameraPos.x, pCamera->cameraPos.y, pCamera->cameraPos.z);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "diffuseTexture"), 0);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "shadowMap"), 1);
        //
        tex1.use(0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        cube1->draw(&sceneShader, pCamera);
        cube2->draw(&sceneShader, pCamera);
        cube3->draw(&sceneShader, pCamera);
        //
        tex1.use(0);
        plane.draw(&sceneShader, pCamera);
        //*/
        glfwSwapBuffers(window);
        fc.update();
    }
    delete cube1;
    delete cube2;
    delete cube3;
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 在点光源下的光照和阴影
void exercise1(){
    GLFWwindow *window = initWindow("ShadowMapping-EX1", SCR_WIDTH, SCR_HEIGHT);
    glfwSwapInterval(0);
    showEnviroment();
    Camera* pCamera = CameraController::getCamera();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);
    //
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    CoordinateAxes ca(pCamera);
    // 生成ShadowMap
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 生成ShadowMap的FBO, 绑定纹理
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "Framebuffer not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 平行光的投影矩阵(正交)
    GLfloat near = 1.0f, far = 7.5f;
    glm::mat4 lightProjection = glm::perspective(45.0f, 4.0f/3.0f, near, far);
    // 平行光的视角矩阵
    glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    // 计算ShadowMap的着色器
    Shader depthMapShader("Resources/Shaders/ShadowMapping/light.vs", "Resources/Shaders/ShadowMapping/light.frag");
    // 可视化ShadowMap的着色器
    Shader debugShader("Resources/Shaders/ShadowMapping/screen.vs", "Resources/Shaders/ShadowMapping/screen.frag");
    // 普通场景的着色器
    Shader sceneShader("Resources/Shaders/ShadowMapping/shadowed_object_point_light.vs", "Resources/Shaders/ShadowMapping/shadowed_object_point_light.frag");
    // 立方体物体
    Shape *cube1 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    Shape *cube2 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    Shape *cube3 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    glm::mat4 tmpModel = glm::mat4();
    tmpModel = glm::translate(tmpModel, glm::vec3(-1.0f, 0.0f, 2.0));
    tmpModel = glm::rotate(tmpModel, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    tmpModel = glm::scale(tmpModel, glm::vec3(0.5));
    cube1->moveTo(cubePositions[0]);
    cube2->moveTo(cubePositions[1]);
    cube3->setModelMat(tmpModel);
    // 地面
    Shape plane(planeVertices, 6, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    // 屏幕
    Shape screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    // 纹理
    Texture tex0("Resources/Textures/container.jpg", GL_BGR, GL_RGB);
    Texture tex1("Resources/Textures/wood.jpg", GL_BGR, GL_RGB);
    //
    FPSCounter fc;
    // MainLoop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        // 从光源处绘制，生成ShadowMap
        glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
        glCullFace(GL_FRONT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            depthMapShader.use();
            // 光源视角矩阵
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "lightSpaceMatrix"),
                               1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            // 绘制地面
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(plane.getModelMat()));
            plane.draw(&depthMapShader);
            //绘制3个立方体
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube1->getModelMat()));
            cube1->draw(&depthMapShader);
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube2->getModelMat()));
            cube2->draw(&depthMapShader);
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube3->getModelMat()));
            cube3->draw(&depthMapShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 第二次绘制，根据ShadowMap绘制场景
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sceneShader.use();
        glUniformMatrix4fv(glGetUniformLocation(sceneShader.programID, "lightSpaceMatrix"),
                           1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform3f(glGetUniformLocation(sceneShader.programID, "lightPosition"),
                    lightPosition.x, lightPosition.y, lightPosition.z);
        glUniform3f(glGetUniformLocation(sceneShader.programID, "viewPosition"),
                    pCamera->cameraPos.x, pCamera->cameraPos.y, pCamera->cameraPos.z);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "diffuseTexture"), 0);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "shadowMap"), 1);
        tex1.use(0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        cube1->draw(&sceneShader, pCamera);
        cube2->draw(&sceneShader, pCamera);
        cube3->draw(&sceneShader, pCamera);
        tex1.use(0);
        plane.draw(&sceneShader, pCamera);
        //
        glfwSwapBuffers(window);
        fc.update();
    }
    delete cube1;
    delete cube2;
    delete cube3;
    glfwDestroyWindow(window);
    glfwTerminate();
}

//
void exercise2(){
    //
    GLFWwindow *window = initWindow("ShadowMapping", SCR_WIDTH, SCR_HEIGHT);
    glfwSwapInterval(0);
    showEnviroment();
    Camera *pCamera = CameraController::getCamera();
    pCamera->moveto(glm::vec3(0.09, 5.88, 5.56));
    CameraController::pitch = -41.30;
    CameraController::yaw = -88.55;
    pCamera->rotate(-41.30, -88.55);
    //
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //
    CoordinateAxes ca(pCamera);
    // 生成ShadowMap
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 生成ShadowMap的FBO, 绑定纹理
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 平行光的投影矩阵(正交)
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    // 平行光的视角矩阵
    glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    // 计算ShadowMap的着色器
    Shader depthMapShader("Resources/Shaders/ShadowMapping/light.vs", "Resources/Shaders/ShadowMapping/light.frag");
    // 可视化ShadowMap的着色器
    Shader debugShader("Resources/Shaders/ShadowMapping/screen.vs", "Resources/Shaders/ShadowMapping/screen.frag");
    // 普通场景的着色器
    Shader sceneShader("Resources/Shaders/ShadowMapping/shadowed_object_directional_light.vs", "Resources/Shaders/ShadowMapping/shadowed_object_directional_light.frag");
    // 立方体物体
    Shape *cube1 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    Shape *cube2 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    Shape *cube3 = new Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    cube1->moveTo(glm::vec3(0.0f, 0.0f, 0.2f));
    cube2->moveTo(glm::vec3(-1.2f, 0.0f, -1.9f));
    cube2->scaleTo(glm::vec3(1.2f, 1.2f, 1.5f));
    cube3->moveTo(glm::vec3(1.5f, 0.0f, -1.5f));
    cube3->scaleTo(glm::vec3(1.7f, 1.7f, 2.0f));
    // 地面
    Shape plane(planeVertices, 6, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    // 屏幕
    Shape screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    // 纹理
    Texture tex0("Resources/Textures/container.jpg", GL_BGR, GL_RGB);
    Texture tex1("Resources/Textures/wood.jpg", GL_BGR, GL_RGB);
    FPSCounter fc;
    ControlPanel panel(window);
    // MainLoop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        // 从光源处绘制，生成ShadowMap
        glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
        glCullFace(GL_FRONT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            depthMapShader.use();
            // 光源视角矩阵
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "lightSpaceMatrix"),
                               1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            // 绘制地面
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(plane.getModelMat()));
            plane.draw(&depthMapShader);
            //绘制3个立方体
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube1->getModelMat()));
            cube1->draw(&depthMapShader);
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube2->getModelMat()));
            cube2->draw(&depthMapShader);
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"),
                               1, GL_FALSE, glm::value_ptr(cube3->getModelMat()));
            cube3->draw(&depthMapShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 第二次绘制，根据ShadowMap绘制场景
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sceneShader.use();
        glUniformMatrix4fv(glGetUniformLocation(sceneShader.programID, "lightSpaceMatrix"),
                           1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform3f(glGetUniformLocation(sceneShader.programID, "lightPosition"),
                    lightPosition.x, lightPosition.y, lightPosition.z);
        glUniform3f(glGetUniformLocation(sceneShader.programID, "viewPosition"),
                    pCamera->cameraPos.x, pCamera->cameraPos.y, pCamera->cameraPos.z);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "diffuseTexture"), 0);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "shadowMap"), 1);
        tex1.use(0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        cube1->draw(&sceneShader, pCamera);
        cube2->draw(&sceneShader, pCamera);
        cube3->draw(&sceneShader, pCamera);
        tex1.use(0);
        plane.draw(&sceneShader, pCamera);
        //
        panel.draw();
        glfwSwapBuffers(window);
        fc.update();
    }
    delete cube1;
    delete cube2;
    delete cube3;
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
GLfloat planeVertices[6*8] = {
    // Positions          // Normals         // Texture Coords
    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
    -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
    -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
    25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
    - 25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
};
};

#endif //SHADOW_MAPPING_HPP
