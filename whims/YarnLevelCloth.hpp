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
    glm::vec3(0.0f, 5.0f,  00.0f),
    glm::vec3(0.0f, 5.0f,  10.0f),
    glm::vec3(0.0f, 5.0f,  20.0f),
    glm::vec3(0.0f, 5.0f,  30.0f),
    glm::vec3(0.0f, 5.0f,  40.0f),
    glm::vec3(0.0f, 5.0f,  50.0f),
    glm::vec3(0.0f, 5.0f,  60.0f),
    glm::vec3(0.0f, 5.0f,  70.0f),
    glm::vec3(0.0f, 5.0f,  80.0f),
    glm::vec3(0.0f, 5.0f,  90.0f),
    glm::vec3(0.0f, 5.0f,  100.0f),
};

// 表示层中心的最大半径
const GLfloat R_ply = 3.0f;
// 控制层中心绕纺线中心的旋转周期
const GLfloat alpha = 1.0f;
// 纺线的法向量
glm::vec3 N_yarn = glm::vec3(0.0, 1.0, 0.0);
// 纺线的双切向量(和切向量法向量平面垂直的向量)
glm::vec3 B_yarn = glm::vec3(1.0, 0.0, 0.0);
//
const GLfloat R_min = 0.0f;
const GLfloat R_max = 5.0f;
//
const GLfloat eN;
const GLfloat eB;

// 根据纺线中心计算层中心
vector<glm::vec3> calcPlyCenter(vector<glm::vec3> yarnCenter, GLfloat theta_Ply) {
    vector<glm::vec3> plyCenter(yarnCenter);
    // 通过z轴反求出 极坐标角度 theta。(`c_i(\theta).z = \altha \theta / 2\pi`)
    for(int i = 0; i < yarnCenter.size(); ++i) {
        // 反求theta
        GLfloat theta = (2.0 * glm::pi<float>() * yarnCenter[i].z) / alpha;
        // 转换成弧度
        theta = theta * 2.0 * glm::pi<float>() / 360.0f;
        // 计算中心相对偏移
        plyCenter[i] = 0.5f * R_ply * ((cos(theta_Ply + theta) * N_yarn) + (sin(theta_Ply + theta) * B_yarn));
        // 叠加纺线中心
        plyCenter[i] = yarnCenter[i] + plyCenter[i];
    }
    return plyCenter;
}

vector<glm::vec3> calcMigrationFiber(vector<glm::vec3> plyCenter) {

}

#define DRAW_MODE GL_LINE_STRIP

//
void singleYarn() {
    // 初始化
    GLFWwindow *window = initWindow("TessellationShader", 800, 600, 4, 0);
    showEnviroment();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2.0);
    CameraController::bindControl(window);
    Camera *cam = &CameraController::camera;
    CameraController::camera.moveto(glm::vec3(25.0f, 5.0f, 50.0f));
    CameraController::yaw = -180.0f;
    CameraController::pitch = 0.0f;
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
    vector<glm::vec3> plyCenter1 = calcPlyCenter(yarnCenter, 1.0f * 2.0f * glm::pi<float>() / 3.0f);
    vector<glm::vec3> plyCenter2 = calcPlyCenter(yarnCenter, 2.0f * 2.0f * glm::pi<float>() / 3.0f);
    vector<glm::vec3> plyCenter3 = calcPlyCenter(yarnCenter, 3.0f * 2.0f * glm::pi<float>() / 3.0f);
    //
    Object *yarn = new Object(&yarnCenter[0].x, yarnCenter.size(), POSITIONS, DRAW_MODE);
    yarn->setCamera(cam);
    Object *ply1 = new Object(&plyCenter1[0].x, plyCenter1.size(), POSITIONS, DRAW_MODE);
    ply1->setCamera(cam);

    Object *ply2 = new Object(&plyCenter2[0].x, plyCenter2.size(), POSITIONS, DRAW_MODE);
    ply2->setCamera(cam);

    Object *ply3 = new Object(&plyCenter3[0].x, plyCenter3.size(), POSITIONS, DRAW_MODE);
    ply3->setCamera(cam);
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
        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 0.0, 1.0, 0.0);
        ply2->setShader(&shader);
        ply2->draw();
        //
        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 0.0, 0.0, 1.0);
        ply3->setShader(&shader);
        ply3->draw();
        //
        panel.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif // YARN_LEVEL_CLOTH_HPP
