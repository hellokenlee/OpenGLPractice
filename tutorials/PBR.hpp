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
}
// 光源颜色/强度
glm::vec3 lightColors[] = {
    glm::vec3(300.0f, 300.0f, 300.0f),
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

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif // PBR_HPP
