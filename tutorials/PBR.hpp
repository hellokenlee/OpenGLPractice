/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef PBR_HPP
#define PBR_HPP
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

    ControlPanel panel(window);

    CameraController::bindControl(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;

    FPSCounter fc;

    Shader pbrShader("shaders/PBR/pbr.vs", "shaders/PBR/pbr.frag");
    Shader sphereShader("shaders/PBR/sphere.vs", "shaders/PBR/sphere.frag");
    Shader normalShader("shaders/PBR/showNormals.vs", "shaders/PBR/showNormals.frag");
    normalShader.addOptionalShader("shaders/PBR/showNormals.geom", GL_GEOMETRY_SHADER);
    Object *sphere = Geometry::icoSphere(2);
    sphere->setShader(&pbrShader);
    sphere->setCamera(cam);

    char uniformNameBuffer[128];
    int rowNum = 7, colNum = 7;



    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //ca.draw();
        pbrShader.use();
        // 写入光源信息
        for(int i = 0; i < 4; ++i){
            sprintf(uniformNameBuffer, "lights[%d].worldPos", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lights[%d].color", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightColors[i]));
        }
        // 写入视角位置
        glUniform3fv(glGetUniformLocation(pbrShader.programID, "viewPos"), 1, glm::value_ptr(cam->cameraPos));
        // 写入不变的材质信息
        glUniform3f(glGetUniformLocation(pbrShader.programID, "material.albedo"), 0.5f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(pbrShader.programID, "material.ao"), 1.0f);
        // 按位置写入材质，位置信息
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for(int r = 0; r < rowNum; ++r){
            glUniform1f(glGetUniformLocation(pbrShader.programID, "material.metallic"), float(r) / float(rowNum));
            for(int c = 0; c < colNum; ++c){
                glUniform1f(glGetUniformLocation(pbrShader.programID, "material.roughness"), glm::clamp(float(c) / float(colNum), 0.05f, 1.0f));
                sphere->moveTo(glm::vec3(2.5 * (c - ((float)colNum / 2.0)), 2.5 * (r - ((float)rowNum / 2.0)), 0.0f));
                sphere->draw();
            }
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //
        panel.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 纹理PBR， 预计算TBN矩阵，手动传入
void exercise1(){
    GLFWwindow *window = initWindow("PBR", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);

    CameraController::bindControl(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;

    FPSCounter fc;

    Shader pbrShader("shaders/PBR/pbr_texture3.vs", "shaders/PBR/pbr_texture3.frag");
    Shader showTBNShader("shaders/PBR/showTBN.vs", "shaders/PBR/showTBN.frag");
    showTBNShader.addOptionalShader("shaders/PBR/showTBN.geom", GL_GEOMETRY_SHADER);

    Object *sphere = Geometry::icoSphere(3);
    sphere->setCamera(cam);

    TextureManager* tm = TextureManager::getManager();
    tm->loadTexture("textures/sphere/rustediron2_basecolor.png", 0, GL_BGRA, GL_RGBA);
    tm->loadTexture("textures/sphere/rustediron2_normal.png", 1, GL_BGRA, GL_RGBA);
    tm->loadTexture("textures/sphere/rustediron2_metallic.png", 2, GL_BGRA, GL_RGBA);
    tm->loadTexture("textures/sphere/rustediron2_roughness.png", 3, GL_BGRA, GL_RGBA);
    tm->loadTexture("textures/sphere/rustediron2_ao.png", 4, GL_BGRA, GL_RGBA);

    char uniformNameBuffer[128];
    int rowNum = 7, colNum = 7;
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
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lights[%d].color", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightColors[i]));
        }
        // 写入视角位置
        glUniform3fv(glGetUniformLocation(pbrShader.programID, "viewPos"), 1, glm::value_ptr(cam->cameraPos));
        //贴图
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        glActiveTexture(GL_TEXTURE2);
        tm->bindTexture(2);
        glActiveTexture(GL_TEXTURE3);
        tm->bindTexture(3);
        glActiveTexture(GL_TEXTURE4);
        tm->bindTexture(4);
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
                sphere->setShader(&pbrShader);
                sphere->draw();
            }
        }

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 纹理PBR， 在片段着色器中计算TBN矩阵
void exercise2(){
    GLFWwindow *window = initWindow("PBR", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);

    CameraController::bindControl(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;

    FPSCounter fc;

    Shader pbrShader("shaders/PBR/pbr_texture2.vs", "shaders/PBR/pbr_texture2.frag");
    Object *sphere = Geometry::icoSphere(4);
    sphere->setShader(&pbrShader);
    sphere->setCamera(cam);


    TextureManager* tm = TextureManager::getManager();
    tm->loadTexture("textures/sphere/rustediron2_basecolor.png", 0, GL_BGRA, GL_RGBA);
    tm->loadTexture("textures/sphere/rustediron2_normal.png", 1, GL_BGRA, GL_RGBA);
    tm->loadTexture("textures/sphere/rustediron2_metallic.png", 2, GL_BGRA, GL_RGBA);
    tm->loadTexture("textures/sphere/rustediron2_roughness.png", 3, GL_BGRA, GL_RGBA);
    tm->loadTexture("textures/sphere/rustediron2_ao.png", 4, GL_BGRA, GL_RGBA);

    char uniformNameBuffer[128];
    int rowNum = 1, colNum = 1;
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
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lightColors[%d]", i);
            glUniform3fv(glGetUniformLocation(pbrShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightColors[i]));
        }
        // 写入视角位置
        glUniform3fv(glGetUniformLocation(pbrShader.programID, "camPos"), 1, glm::value_ptr(cam->cameraPos));
        //贴图
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        glActiveTexture(GL_TEXTURE2);
        tm->bindTexture(2);
        glActiveTexture(GL_TEXTURE3);
        tm->bindTexture(3);
        glActiveTexture(GL_TEXTURE4);
        tm->bindTexture(4);
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
                sphere->draw();
            }
        }

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif // PBR_HPP
