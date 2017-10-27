/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef YARN_LEVEL_CLOTH_HPP
#define YARN_LEVEL_CLOTH_HPP

#include <cmath>

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

vector<glm::vec3> yarnCenter = {
    glm::vec3(0.0f, 3.0f,  0.0f),
    glm::vec3(0.0f, 3.0f,  2.0f),
    glm::vec3(0.0f, 3.0f,  4.0f),
    glm::vec3(0.0f, 3.0f,  6.0f)
};

const GLfloat R_ply = 1.0f;
const GLfloat alpha = 1.0f;
glm::vec3 N_yarn = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 B_yarn = glm::vec3(1.0, 0.0, 0.0);

vector<glm::vec3> calcPlyCenter(vector<glm::vec3> yarnCenter, GLfloat theta_Ply) {
    vector<glm::vec3> plyCenter(yarnCenter);
    // 通过z轴反求出 极坐标角度 theta。(`c_i(\theta).z = \altha \theta / 2\pi`)
    for(int i = 0; i < yarnCenter.size(); ++i) {
        GLfloat theta = (2.0 * glm::pi<float>() * yarnCenter[i].z) / alpha;
        theta = theta * 2.0 * glm::pi<float>() / 360.0f;
        plyCenter[i] = 0.5f * R_ply * ((cos(theta_Ply + theta) * N_yarn) + (sin(theta_Ply + theta) * B_yarn));
        plyCenter[i].z = yarnCenter[i].z;
        //printf("(%f, %f, %f), theta = %f\n", plyCenter[i].x, plyCenter[i].y, plyCenter[i].z, theta);
    }
    return plyCenter;
}
//
void singleYarn() {
    // 初始化
    GLFWwindow *window = initWindow("TessellationShader", 800, 600, 4, 0);
    showEnviroment();
    glEnable(GL_PROGRAM_POINT_SIZE);
    //glPointSize(4.0);
    CameraController::bindControl(window);
    Camera *cam = &CameraController::camera;
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
    Shader shader("shaders/YarnLevelCloth/color.vert", "shaders/YarnLevelCloth/color.frag");
    //
    yarnCenter = Curve::CRChain(yarnCenter, 100);
    vector<glm::vec3> plyCenter = calcPlyCenter(yarnCenter, 1.0f * 2.0f * glm::pi<float>() / 3.0f);
    //
    Object *yarn = new Object(&yarnCenter[0].x, yarnCenter.size(), POSITIONS, GL_POINTS);
    yarn->setCamera(cam);
    Object *ply1 = new Object(&plyCenter[0].x, plyCenter.size(), POSITIONS, GL_POINTS);
    ply1->setCamera(cam);
    // 主循环
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        ca.draw();
        //
        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 1.0, 1.0, 1.0);
        yarn->setShader(&shader);
        yarn->draw();
        //
        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 1.0, 0.0, 0.0);
        ply1->setShader(&shader);
        ply1->draw();
        //
        panel.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif // YARN_LEVEL_CLOTH_HPP
