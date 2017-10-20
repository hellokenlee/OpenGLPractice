/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef PN_TRIANGLE_HPP
#define PN_TRIANGLE_HPP

namespace PNTriangle {
// 三角形顶点
GLfloat vertices[] = {
    // Positions        // Normals
     0.0f,  0.5f, 0.0f, 0.0f, 0.5f, 1.0f,  // Top
     0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Bottom Left
};

// 主函数
void _main() {
    // 初始化
    GLFWwindow *window = initWindow("TessellationShader", 800, 600, 4, 0);
    //glfwSwapInterval(0);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(4.0);
    showEnviroment();
    // 输出最大支持的细分Patch
    int patchVerticesNum, maxPatchVerticesNum;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVerticesNum);
    cout<<"Maximun Vertices in Each Patch Supported: "<<maxPatchVerticesNum<<endl<<endl;
    // 设定每一个Patch中有多少个顶点，假设是三角形，所有每个Patch用3个顶点
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glGetIntegerv(GL_PATCH_VERTICES, &patchVerticesNum);
    cout<<"Vertices in Each Patch has been Setted to: "<<patchVerticesNum<<endl<<endl;
    // 控制绑定
    CameraController::bindControl(window);
    Camera *cam = &CameraController::camera;
    // 辅助
    CoordinateAxes ca(cam);
    ControlPanel panel(window);
    // 初始化着色器
    Shader shader("shaders/PNTriangle/pn.vert", "shaders/PNTriangle/pn.frag");
    shader.addOptionalShader("shaders/PNTriangle/pn.tesc", GL_TESS_CONTROL_SHADER);
    shader.addOptionalShader("shaders/PNTriangle/pn.tese", GL_TESS_EVALUATION_SHADER);
    Shader shader2("shaders/Share/Color.vert", "shaders/Share/Color.frag");
    // 模型
    Object triangle(vertices, 3, POSITIONS_NORMALS, GL_TRIANGLES);
    triangle.scaleTo(2.0f);
    triangle.setCamera(cam);
    triangle.setDrawMode(GL_PATCHES);

    Model monkey("textures/untitled.obj");
    monkey.setCamera(cam);
    //monkey.setShader(&shader2);
    monkey.setDrawMode(GL_PATCHES);

    // 主循环
    while(!glfwWindowShouldClose(window)) {
        //
        glfwPollEvents();
        CameraController::update();
        //
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        //ca.draw();
        panel.draw();
        //
        shader.use();
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "model"), 1, GL_FALSE, glm::value_ptr(triangle.model));
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "view"), 1, GL_FALSE, cam->getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "projection"), 1, GL_FALSE, cam->getProjectionMatrixVal());
        //triangle.draw();
        monkey.draw();
        //*/
        glfwSwapBuffers(window);
    }
    // 清理
    glfwDestroyWindow(window);
    glfwTerminate();

}
};
#endif // PN_TRIANGLE_HPP

