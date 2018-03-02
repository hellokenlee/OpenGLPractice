/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef PN_TRIANGLE_HPP
#define PN_TRIANGLE_HPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

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
    Shader shader("Resources/Shaders/PNTriangle/pn.vert", "Resources/Shaders/PNTriangle/pn.frag");
    shader.addOptionalShader("Resources/Shaders/PNTriangle/pn.tesc", GL_TESS_CONTROL_SHADER);
    shader.addOptionalShader("Resources/Shaders/PNTriangle/pn.tese", GL_TESS_EVALUATION_SHADER);

    Shader shader2("Resources/Shaders/GeometryShader/showNormals.vs", "Resources/Shaders/GeometryShader/showNormals.frag");
    shader2.addOptionalShader("Resources/Shaders/GeometryShader/showNormals.geom", GL_GEOMETRY_SHADER);

    Shader shader3("Resources/Shaders/Share/Color.vert", "Resources/Shaders/Share/Color.frag");
    // 模型
    Shape triangle(vertices, 3, POSITIONS_NORMALS, GL_TRIANGLES);
    triangle.scaleTo(2.0f);
    triangle.setCamera(cam);
    triangle.setDrawMode(GL_PATCHES);

    Model monkey("Resources/Textures/monkey2.obj");
    monkey.setCamera(cam);
    //monkey.setDrawMode(GL_PATCHES);

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
        //*Debug Showing Normals
        panel.draw();
        monkey.setShader(&shader2);
        monkey.draw();
        monkey.setShader(&shader3);
        monkey.draw();
        //*/
        /*
        shader.use();
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "model"), 1, GL_FALSE, glm::value_ptr(triangle.model));
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "view"), 1, GL_FALSE, cam->getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "projection"), 1, GL_FALSE, cam->getProjectionMatrixVal());
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

