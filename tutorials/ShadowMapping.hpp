/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef SHADOW_MAPPING_HPP
#define SHADOW_MAPPING_HPP
namespace ShadowMapping{

// 顶点信息前置声明
extern GLfloat screenVertices[6*5];
extern GLfloat cubeVertices[36*5];
extern GLfloat planeVertices[6*5];
// 立方体位置
glm::vec3 cubePositions[2]={glm::vec3(-1.0f, 0.0f, -1.0f),glm::vec3(2.0f, 0.0f, 0.0f)};
// ShadowMap的解析度
const GLuint SHADOW_MAP_WIDTH = 1024;
const GLuint SHADOW_MAP_HEIGHT = 1024;

// 教程实现
void tutorial(){
    GLFWwindow *window = initWindow("ShadowMapping", 800, 600);
    showEnviroment();

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //CameraController::bindControl(window);

    glEnable(GL_DEPTH_TEST);

    CoordinateAxes ca(&CameraController::camera);

    // 生成ShadowMap
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
    glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    // 计算ShadowMap的着色器
    Shader depthMapShader("shaders/ShadowMapping/light.vs", "shaders/ShadowMapping/light.frag");
    // 可视化ShadowMap的着色器
    Shader debugShader("shaders/ShadowMapping/screen.vs", "shaders/ShadowMapping/screen.frag");
    // 普通场景的着色器
    Shader sceneShader("shaders/ShadowMapping/scene.vs", "shaders/ShadowMapping/scene.frag");
    // 立方体物体
    Object *cube1 = new Object(cubeVertices, 36, POSITIONS_TEXTURES, GL_TRIANGLES);
    cube1->setCamera(&CameraController::camera);
    //cube1->setShader(&sceneShader);
    Object *cube2 = cube1->clone();
    cube1->moveTo(cubePositions[0]);
    cube2->moveTo(cubePositions[1]);
    // 地面
    Object plane(planeVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    //plane.setShader(&sceneShader);
    plane.setCamera(&CameraController::camera);
    // 屏幕
    Object screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    screen.setShader(&debugShader);

    CameraController::camera.cameraPos = glm::vec3(-2.0f, 4.0f, -1.0f);
    CameraController::camera.cameraFront = glm::vec3(2.0f, -4.0f, 1.0f);
    CameraController::camera.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    CameraController::camera.projection = lightProjection;
    CameraController::camera.update();


    // MainLoop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        //CameraController::update();


        glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            depthMapShader.use();
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(cube1->model));
            cube1->draw();
            depthMapShader.use();
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(cube2->model));
            cube2->draw();
            depthMapShader.use();
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(plane.model));
            plane.draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, 800, 600);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        screen.draw();

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

#endif //SHADOW_MAPPING_HPP
