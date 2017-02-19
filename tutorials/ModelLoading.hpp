/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MODEL_LOADING_HPP
#define MODEL_LOADING_HPP
namespace ModelLoading{

//显示程序
void tutorial(){
    GLFWwindow *window=initWindow("ModelLoading",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);
    //着色器初始化
    Shader modelShader("shaders/ModelLoading/nanosuit.vs","shaders/ModelLoading/nanosuit.frag");
    //模型初始化
    Model model("textures/nanosuit/nanosuit.obj");
    model.setShader(&modelShader);
    model.setCamera(&CameraController::camera);
    //绑定控制
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //绘制
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        CameraController::update();

        ca.draw();
        model.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif // MODEL_LOADING_HPP
