/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef SHADOW_MAPPING_HPP
#define SHADOW_MAPPING_HPP
namespace ShadowMapping{

// 顶点信息前置声明
extern GLfloat screenVertices[6*5];
extern GLfloat cubeVertices[6*6*8];
extern GLfloat planeVertices[6*8];
// 立方体位置
glm::vec3 cubePositions[2] = {glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(2.0f, 0.0f, 1.0f)};
// ShadowMap的解析度
const GLuint SHADOW_MAP_WIDTH = 2048;
const GLuint SHADOW_MAP_HEIGHT = 2048;
glm::vec3 lightPosition = glm::vec3(-2.0f, 4.0f, -1.0f);

// 教程实现: 在定向光下物体光照和阴影
void tutorial(){
    GLFWwindow *window = initWindow("ShadowMapping", 800, 600);
    glfwSwapInterval(0);
    showEnviroment();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    CoordinateAxes ca(&CameraController::camera);

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
    Shader depthMapShader("shaders/ShadowMapping/light.vs", "shaders/ShadowMapping/light.frag");
    // 可视化ShadowMap的着色器
    Shader debugShader("shaders/ShadowMapping/screen.vs", "shaders/ShadowMapping/screen.frag");
    // 普通场景的着色器
    Shader sceneShader("shaders/ShadowMapping/shadowed_object_directional_light.vs", "shaders/ShadowMapping/shadowed_object_directional_light.frag");
    // 立方体物体
    Object *cube1 = new Object(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    cube1->setCamera(&CameraController::camera);
    //cube1->setShader(&sceneShader);
    Object *cube2 = cube1->clone();
    Object *cube3 = cube1->clone();
    cube3->model = glm::mat4();
    cube3->model = glm::translate(cube3->model, glm::vec3(-1.0f, 0.0f, 2.0));
    cube3->model = glm::rotate(cube3->model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    cube3->model = glm::scale(cube3->model, glm::vec3(0.5));
    cube1->moveTo(cubePositions[0]);
    cube2->moveTo(cubePositions[1]);
    // 地面
    Object plane(planeVertices, 6, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    //plane.setShader(&sceneShader);
    plane.setCamera(&CameraController::camera);
    // 屏幕
    Object screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    screen.setShader(&debugShader);
    // 纹理
    Camera *cam = &CameraController::camera;
    TextureManager *tm = TextureManager::getManager();
    if(!tm->loadTexture("textures/container.jpg",0,GL_BGR,GL_RGB))
        return ;
    if(!tm->loadTexture("textures/wood.jpg",1,GL_BGR,GL_RGB))
        return ;
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
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            // 绘制地面
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(plane.model));
            plane.draw();
            //绘制3个立方体
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(cube1->model));
            cube1->draw();
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(cube2->model));
            cube2->draw();
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(cube3->model));
            cube3->draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 第二次绘制，根据ShadowMap绘制场景
        glViewport(0, 0, 800, 600);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sceneShader.use();
        glUniformMatrix4fv(glGetUniformLocation(sceneShader.programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform3f(glGetUniformLocation(sceneShader.programID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
        glUniform3f(glGetUniformLocation(sceneShader.programID, "viewPosition"), cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "diffuseTexture"), 0);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "shadowMap"), 1);
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        cube1->setShader(&sceneShader);
        cube2->setShader(&sceneShader);
        cube3->setShader(&sceneShader);
        cube1->draw();
        cube2->draw();
        cube3->draw();
        cube1->setShader(nullptr);
        cube2->setShader(nullptr);
        cube3->setShader(nullptr);
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(1);
        plane.setShader(&sceneShader);
        plane.draw();
        plane.setShader(nullptr);
        //ca.draw();
        glfwSwapBuffers(window);
        fc.update();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

// 在点光源下的光照和阴影
void exercise1(){
    GLFWwindow *window = initWindow("ShadowMapping", 800, 600);
    glfwSwapInterval(0);
    showEnviroment();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    CoordinateAxes ca(&CameraController::camera);

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
    Shader depthMapShader("shaders/ShadowMapping/light.vs", "shaders/ShadowMapping/light.frag");
    // 可视化ShadowMap的着色器
    Shader debugShader("shaders/ShadowMapping/screen.vs", "shaders/ShadowMapping/screen.frag");
    // 普通场景的着色器
    Shader sceneShader("shaders/ShadowMapping/shadowed_object_point_light.vs", "shaders/ShadowMapping/shadowed_object_point_light.frag");
    // 立方体物体
    Object *cube1 = new Object(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    cube1->setCamera(&CameraController::camera);
    //cube1->setShader(&sceneShader);
    Object *cube2 = cube1->clone();
    Object *cube3 = cube1->clone();
    cube3->model = glm::mat4();
    cube3->model = glm::translate(cube3->model, glm::vec3(-1.0f, 0.0f, 2.0));
    cube3->model = glm::rotate(cube3->model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    cube3->model = glm::scale(cube3->model, glm::vec3(0.5));
    cube1->moveTo(cubePositions[0]);
    cube2->moveTo(cubePositions[1]);
    // 地面
    Object plane(planeVertices, 6, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    //plane.setShader(&sceneShader);
    plane.setCamera(&CameraController::camera);
    // 屏幕
    Object screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    screen.setShader(&debugShader);
    // 纹理
    Camera *cam = &CameraController::camera;
    TextureManager *tm = TextureManager::getManager();
    if(!tm->loadTexture("textures/container.jpg",0,GL_BGR,GL_RGB))
        return ;
    if(!tm->loadTexture("textures/wood.jpg",1,GL_BGR,GL_RGB))
        return ;
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
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            // 绘制地面
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(plane.model));
            plane.draw();
            //绘制3个立方体
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(cube1->model));
            cube1->draw();
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(cube2->model));
            cube2->draw();
            glUniformMatrix4fv(glGetUniformLocation(depthMapShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(cube3->model));
            cube3->draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 第二次绘制，根据ShadowMap绘制场景
        glViewport(0, 0, 800, 600);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sceneShader.use();
        glUniformMatrix4fv(glGetUniformLocation(sceneShader.programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform3f(glGetUniformLocation(sceneShader.programID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
        glUniform3f(glGetUniformLocation(sceneShader.programID, "viewPosition"), cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "diffuseTexture"), 0);
        glUniform1i(glGetUniformLocation(sceneShader.programID, "shadowMap"), 1);
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        cube1->setShader(&sceneShader);
        cube2->setShader(&sceneShader);
        cube3->setShader(&sceneShader);
        cube1->draw();
        cube2->draw();
        cube3->draw();
        cube1->setShader(nullptr);
        cube2->setShader(nullptr);
        cube3->setShader(nullptr);
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(1);
        plane.setShader(&sceneShader);
        plane.draw();
        plane.setShader(nullptr);
        //ca.draw();
        glfwSwapBuffers(window);
        fc.update();
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
