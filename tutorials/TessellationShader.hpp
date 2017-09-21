/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef TESSELLATION_SHADER
#define TESSELLATION_SHADER
namespace TessellationShader {

void _main() {
    // 初始化
    GLFWwindow *window = initWindow("TessellationShader", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    // 控制绑定
    CameraController::bindControl(window);
    Camera *cam = &CameraController::camera;
    // 辅助
    CoordinateAxes ca(cam);
    ControlPanel panel(window);
    // 主循环
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 0, 1);
        glVertex3f(0, 0, 0);
        glEnd();
        //ca.draw();
        //panel.draw();

        glfwSwapBuffers(window);
    }
    // 清理
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif // TESSELLATION_SHADER
