/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef YARN_LEVEL_CLOTH_HPP
#define YARN_LEVEL_CLOTH_HPP

#include <math.h>

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



float ti(float ti_1, glm::vec3 &Pi_1, glm::vec3 &Pi) {
    //float alpha = 0.5f;
    float dx = Pi.x - Pi_1.x;
    float dy = Pi.y - Pi_1.y;
    float dz = Pi.z - Pi_1.z;
    return sqrt((dx * dx) + (dy * dy) + (dz * dz)) + ti_1;
}

void catmullRomSpline(vector<glm::vec3> &container, glm::vec3 &P0, glm::vec3 &P1, glm::vec3 &P2, glm::vec3 &P3, int nPoints = 100) {
    float t0 = 0.0f;
    float t1 = ti(t0, P0, P1);
    float t2 = ti(t1, P1, P2);
    float t3 = ti(t2, P2, P3);

    const float dt = (t2 - t1) / float(nPoints);
    for(float t = t1; t < t2; t += dt) {
        glm::vec3 A1 = ((t1 - t) / (t1 - t0) * P0) + (((t - t0) / (t1 - t0)) * P1);
        glm::vec3 A2 = ((t2 - t) / (t2 - t1) * P1) + (((t - t1) / (t2 - t1)) * P2);
        glm::vec3 A3 = ((t3 - t) / (t3 - t2) * P2) + (((t - t2) / (t3 - t2)) * P3);

        glm::vec3 B1 = ((t2 - t) / (t2 - t0) * A1) + (((t - t0) / (t2 - t0)) * A2);
        glm::vec3 B2 = ((t1 - t) / (t1 - t0) * P0) + (((t - t0) / (t1 - t0)) * P1);

        glm::vec3 C = ((t2 - t) / (t2 - t1) * B1) + (((t - t1) / (t2 - t1)) * B2);

        container.push_back(C);
    }
}

vector<glm::vec3> catmullRomChain(vector<glm::vec3> controlPoints) {
    vector<glm::vec3> res;
    for(unsigned int  i = 0; i < controlPoints.size() - 3; ++i) {
        catmullRomSpline(res, controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3]);
    }
    return res;
}

void testCatmullRomSpline() {
    // 初始化
    GLFWwindow *window = initWindow("Yarn_level_Cloth", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;
    ControlPanel panel(window);
    //
    // 简单着色器
    Shader whiteShader("shaders/Share/Color.vert", "shaders/Share/Color.frag");
    //
    vector<glm::vec3> CPs = {
        glm::vec3(0.0f, 1.5f, 0.0f),
        glm::vec3(2.0f, 2.0f, 0.0f),
        glm::vec3(3.0f, 1.0f, 0.0f),
        glm::vec3(4.0f, 0.5f, 0.0f),
        glm::vec3(5.0f, 1.0f, 0.0f),
        glm::vec3(6.0f, 2.0f, 0.0f),
        glm::vec3(7.0f, 3.0f, 0.0f)
    };
    vector<glm::vec3> points = catmullRomChain(CPs);
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        panel.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}



};
#endif // YARN_LEVEL_CLOTH_HPP
