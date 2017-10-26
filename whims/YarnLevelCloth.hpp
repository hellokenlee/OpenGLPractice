/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef YARN_LEVEL_CLOTH_HPP
#define YARN_LEVEL_CLOTH_HPP

namespace YarnLevelCloth {

// 模拟织线级别的衣物
void _main() {
    // 初始化
    GLFWwindow *window = initWindow("Yarn_level_Cloth", 800, 600,3 ,3);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;
    ControlPanel panel(window);
    FPSCounter fc;
    // 初始化一个曲线集合
    Union *bcc = CurveCollection::genFromBBCFile("textures/Yarn-level Cloth Models/openwork_trellis_pattern.bcc");
    bcc->isShareModel= true;
    // 简单着色器
    Shader whiteShader("shaders/Share/Color.vert", "shaders/Share/Color.frag");
    bcc->setCamera(cam);
    bcc->setShader(&whiteShader);
    // 主循环
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        panel.draw();

        bcc->draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

vector<glm::vec3> vertices = {
    glm::vec3(-3.0f, 1.0f, 0.0f),
    glm::vec3( 3.0f, 1.0f, 0.0f),
    glm::vec3(-3.0f, 3.0f, 0.0f),
    glm::vec3( 3.0f, 3.0f, 0.0f),
    glm::vec3(-3.0f, 5.0f, 0.0f),
    glm::vec3( 3.0f, 5.0f, 0.0f),
    glm::vec3(-3.0f, 7.0f, 0.0f),
    glm::vec3( 3.0f, 7.0f, 0.0f),
    glm::vec3(-3.0f, 9.0f, 0.0f),
    glm::vec3( 3.0f, 9.0f, 0.0f)
};

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
//
void singleYarn() {
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
    cout<<"Vertices in Each Patch has been Setted to: "<<patchVerticesNum<<endl;
    // 着色器
    Shader pointShader("shaders/Share/Color.vert", "shaders/Share/Color.frag");
    //
    Shader shader("shaders/YarnLevelCloth/yarn.vert", "shaders/YarnLevelCloth/yarn.frag");
    shader.addOptionalShader("shaders/YarnLevelCloth/yarn.tesc", GL_TESS_CONTROL_SHADER);
    shader.addOptionalShader("shaders/YarnLevelCloth/yarn.tese", GL_TESS_EVALUATION_SHADER);
    //
    Object *yarn = new Object(&vertices[0].x, vertices.size(), POSITIONS, GL_LINES);
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
#endif // YARN_LEVEL_CLOTH_HPP
