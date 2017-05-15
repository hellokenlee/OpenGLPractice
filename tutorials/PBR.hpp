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
    glm::vec3(200.0f, 200.0f, 200.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f)
};


// 使用 Cook-Torrance BRDF 渲染， 这章的重要实现都在Shader中， 请注意。
void tutorial(){
    GLFWwindow *window = initWindow("HDR", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);

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
    Object *sphere = Geometry::icoSphere(5);
    sphere->setShader(&sphereShader);
    sphere->setCamera(cam);
    sphere->moveTo(glm::vec3(0.0f, 1.0f, 0.0f));

    char uniformNameBuffer[128];
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        sphereShader.use();
        for(int i = 0; i < 1; ++i){
            sprintf(uniformNameBuffer, "lightPositions[%d]", i);
            glUniform3fv(glGetUniformLocation(sphereShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightPositions[i]));
            sprintf(uniformNameBuffer, "lightColors[%d]", i);
            glUniform3fv(glGetUniformLocation(sphereShader.programID, uniformNameBuffer), 1, glm::value_ptr(lightColors[i]));
        }
        glUniform3fv(glGetUniformLocation(sphereShader.programID, "fViewPosition"), 1, glm::value_ptr(cam->cameraPos));
        glUniform1i(glGetUniformLocation(sphereShader.programID, "lightNum"), 1);
        sphere->setShader(&sphereShader);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        sphere->draw();


        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif // PBR_HPP
