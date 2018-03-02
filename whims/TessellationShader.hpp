/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef TESSELLATION_SHADER_HPP
#define TESSELLATION_SHADER_HPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace TessellationShader {

GLfloat vertices[] = {
    // Positions        // Normals
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top
     0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Bottom Left
};

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
    cout<<"Vertices in Each Patch has been Setted to: "<<patchVerticesNum<<endl;
    // 控制绑定
    CameraController::bindControl(window);
    Camera *cam = &CameraController::camera;
    // 辅助
    CoordinateAxes ca(cam);
    ControlPanel panel(window);
    // 初始化着色器
    Shader shader("Resources/Shaders/TessellationShader/tes.vert", "Resources/Shaders/TessellationShader/tes.frag");
    shader.addOptionalShader("Resources/Shaders/TessellationShader/tes.tesc", GL_TESS_CONTROL_SHADER);
    shader.addOptionalShader("Resources/Shaders/TessellationShader/tes.tese", GL_TESS_EVALUATION_SHADER);
    Shader shader2("Resources/Shaders/Share/Color.vert", "Resources/Shaders/Share/Color.frag");
    // 模型
    Shape triangle(vertices, 3, POSITIONS_NORMALS, GL_TRIANGLES);
    triangle.scaleTo(2.0f);
    triangle.setCamera(cam);
    triangle.setDrawMode(GL_PATCHES);
    Model quad("Resources/Textures/quad2.obj");
    quad.setCamera(cam);
    quad.setDrawMode(GL_PATCHES);
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
        glUniform3f(glGetUniformLocation(shader.programID, "eyePos"), cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
        triangle.draw();
        quad.draw();
        //*/
        glfwSwapBuffers(window);
    }
    // 清理
    glfwDestroyWindow(window);
    glfwTerminate();
}


// 顶点坐标
vector<glm::vec3> vertices2 = {
    glm::vec3(-3.0f, 1.0f, 0.0f),
    glm::vec3( 3.0f, 1.0f, 0.0f),

    glm::vec3(-3.0f, 3.0f, 0.0f),
    glm::vec3( 3.0f, 3.0f, 0.0f),

    glm::vec3(-3.0f, 5.0f, 0.0f),
    glm::vec3( 3.0f, 5.0f, 0.0f),

    glm::vec3(-3.0f, 7.0f, 0.0f),
    glm::vec3( 3.0f, 7.0f, 0.0f)
};

// 细分级别
GLfloat tessLevelOuter0_uniform = 1;

// 重写回调
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    CameraController::keyCallback(window, key, scancode, action, mode);
    if(key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        ++tessLevelOuter0_uniform;
        cout<<"gl_TessLevelOuter[0] = "<<tessLevelOuter0_uniform<<endl;
    }
    if(key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        --tessLevelOuter0_uniform;
        cout<<"gl_TessLevelOuter[0] = "<<tessLevelOuter0_uniform<<endl;
    }
}

void isolines() {
    // 初始化
    GLFWwindow *window = initWindow("TessellationShader", 800, 600, 4, 0);
    showEnviroment();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(4.0);
    CameraController::bindControl(window);
    Camera *cam = &CameraController::camera;
    glfwSetKeyCallback(window, keyCallback);
    CoordinateAxes ca(cam);
    ControlPanel panel(window);
    // 输出最大支持的细分Patch
    int patchVerticesNum, maxPatchVerticesNum;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVerticesNum);
    cout<<"Maximun Vertices in Each Patch Supported: "<<maxPatchVerticesNum<<endl<<endl;
    // 设定每一个Patch中有多少个顶点，假设是三角形，所有每个Patch用3个顶点
    ///!!!
    glPatchParameteri(GL_PATCH_VERTICES, 2);
    glGetIntegerv(GL_PATCH_VERTICES, &patchVerticesNum);
    cout<<"Vertices in Each Patch has been Setted to: "<<patchVerticesNum<<endl<<endl;
    // 着色器
    Shader pointShader("Resources/Shaders/TessellationShader/color.vert", "Resources/Shaders/TessellationShader/color.frag");
    //
    Shader shader("Resources/Shaders/TessellationShader/isoline.vert", "Resources/Shaders/TessellationShader/isoline.frag");
    shader.addOptionalShader("Resources/Shaders/TessellationShader/isoline.tesc", GL_TESS_CONTROL_SHADER);
    shader.addOptionalShader("Resources/Shaders/TessellationShader/isoline.tese", GL_TESS_EVALUATION_SHADER);
    //
    Shape *yarn = new Shape(&vertices2[0].x, vertices2.size(), POSITIONS, GL_LINES);
    yarn->setCamera(cam);
    // 主循环
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        ca.draw();
        //
        yarn->setDrawMode(GL_POINTS);
        yarn->setShader(&pointShader);
        yarn->draw();
        //
        shader.use();
        glUniform1f(glGetUniformLocation(shader.programID, "tessLevelOuter0"), tessLevelOuter0_uniform);
        yarn->setDrawMode(GL_PATCHES);
        yarn->setShader(&shader);
        yarn->draw();
        //
        panel.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif // TESSELLATION_SHADER_HPP
