/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef TESSELLATION_SHADER
#define TESSELLATION_SHADER
#define CHECK_GL_ERROR
namespace TessellationShader {

GLfloat vertices[] = {
     0.0f,  0.5f, 0.0f,  // Top
     0.5f, -0.5f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f,  // Bottom Left
};

void _main() {
    // 初始化
    GLFWwindow *window = initWindow("TessellationShader", 800, 600, 4, 0);
    showEnviroment();
    // 输出最大支持的细分Patch
    int maxPatchNum, maxVerticesNum;
    glGetIntegerv(GL_PATCH_VERTICES, &maxPatchNum);
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxVerticesNum);
    cout<<"Maximun Tessellation Patches Supported: "<<maxPatchNum<<endl;
    cout<<"Maximun Vertices in Each Patch Supported: "<<maxVerticesNum<<endl<<endl;
    // 设定每一个Patch中有多少个顶点，假设是三角形，所有每个Patch用3个顶点
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    // 控制绑定
    CameraController::bindControl(window);
    Camera *cam = &CameraController::camera;
    // 辅助
    CoordinateAxes ca(cam);
    ControlPanel panel(window);
    //
    Shader shader("shaders/TessellationShader/tes.vert", "shaders/TessellationShader/tes.frag");
    shader.addOptionalShader("shaders/TessellationShader/tes.tesc", GL_TESS_CONTROL_SHADER);
    shader.addOptionalShader("shaders/TessellationShader/tes.tese", GL_TESS_EVALUATION_SHADER);
    checkError();
    //
    Object triangle(vertices, 3, POSITIONS, GL_POINTS);
    triangle.setShader(&shader);
    triangle.setCamera(cam);
    // 主循环
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();
        panel.draw();
        triangle.draw();

        glfwSwapBuffers(window);
    }
    // 清理
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif // TESSELLATION_SHADER
