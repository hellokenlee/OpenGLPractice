/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef YARN_LEVEL_CLOTH_HPP
#define YARN_LEVEL_CLOTH_HPP
namespace YarnLevelCloth{

// 模拟织线级别的衣物
void test(){
    // 初始化
    GLFWwindow *window = initWindow("Yarn_level_Cloth", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;
    ControlPanel panel(window);
    FPSCounter fc;
    // 初始化一个曲线集合
    Union *bcc = CurveCollection::genFromBBCFile("textures/Yarn-level Cloth Models/tea_cozy.bcc");
    bcc->isShareModel= true;
    cout<<bcc->objList.size()<<endl;
    // 简单着色器
    Shader whiteShader("shaders/Share/Color.vert", "shaders/Share/Color.frag");
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        panel.draw();

        whiteShader.use();
        glUniformMatrix4fv(glGetUniformLocation(whiteShader.programID, "view"), 1, GL_FALSE, cam->getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(whiteShader.programID, "projection"), 1, GL_FALSE, cam->getProjectionMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(whiteShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
        bcc->draw(&whiteShader);
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif // YARN_LEVEL_CLOTH_HPP
