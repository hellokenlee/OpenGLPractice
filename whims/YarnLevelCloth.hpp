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
    glm::vec3(0.0f, 5.0f,  110.0f),
    glm::vec3(0.0f, 5.0f,  120.0f),
    glm::vec3(0.0f, 5.0f,  130.0f),
    glm::vec3(0.0f, 5.0f,  140.0f),
    glm::vec3(0.0f, 5.0f,  150.0f),
    glm::vec3(0.0f, 5.0f,  160.0f),
    glm::vec3(0.0f, 5.0f,  170.0f),
    glm::vec3(0.0f, 5.0f,  180.0f),
    glm::vec3(0.0f, 5.0f,  190.0f),
    glm::vec3(0.0f, 5.0f,  200.0f),
};

// 表示层中心的最大半径
const GLfloat R_ply = 2.0f;
// 控制层中心绕纺线中心的旋转周期
const GLfloat alpha = 1.0f;
const GLfloat alpha_hair = 0.233f;
// 纺线的法向量
glm::vec3 N_yarn = glm::vec3(0.0, 1.0, 0.0);
// 纺线的双切向量(和切向量法向量平面垂直的向量)
glm::vec3 B_yarn = glm::vec3(1.0, 0.0, 0.0);

glm::vec3 T_yarn = glm::vec3(0.0, 0.0, 1.0);
//
const GLfloat R_min = 0.1f;
const GLfloat R_max = 4.0f;
const GLfloat R_loop_max = 2.0 * R_max;
//
const GLfloat e_N = 1.5f;
const GLfloat e_B = 1.0f;
//
const GLfloat s = 1.0;

// 根据纺线中心计算层中心
// `\Delta c_j^{ply} (\theta) = 0.5 R^{ply} (cos(\theta_j^{ply} + \theta) N^{yarn} + sin(\theta_j^{ply} + \theta) B^{yarn})`
vector<glm::vec3> calcPlyCenter(vector<glm::vec3> &yarnCenter, GLfloat theta_Ply) {
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
enum FiberType {
    MIGRATION,
    HAIR,
    LOOP
};
// 根据纺线中心和层中心 计算每一根Fiber的中心位置
vector<vector<glm::vec3>> calcMigrationFiber(vector<glm::vec3> &plyCenter, vector<glm::vec3> &yarnCenter) {
    // 生成 theta_i 和 R_i
    vector<GLfloat> R_initial;
    vector<GLfloat> theta_initial;
    int crossSectionFiberNum = 3;
    GLfloat R_now = 0.01;
    for(int k = 0; k < 6; ++k) {
        for(int l = 0; l < crossSectionFiberNum; ++l) {
            theta_initial.push_back(2.0f * glm::pi<float>() / (GLfloat)crossSectionFiberNum * (GLfloat) l);
            R_initial.push_back(R_now);
        }
        R_now += 0.05f;
        crossSectionFiberNum *= 2;
    }
    //
    vector<vector<glm::vec3>> fiberCenter(R_initial.size(), vector<glm::vec3>(plyCenter.size() * 2));
    for(int i = 0; i < R_initial.size(); ++i) {
        FiberType ft;
        int tmp = rand() % 1000;
        GLfloat _R_max, _R_min;
        GLfloat _alpha = alpha;
        if(tmp < 20){
            // 模拟 Hair-Fiber： 概率 0.5%
            ft = HAIR;
            _R_max = R_max;
            _R_max = R_max + (rand()/(double)(RAND_MAX) * 4.0f);
            _R_min = R_min;
            _alpha = alpha_hair;
            cout<<_R_max<<endl;
        } else if(tmp < 30) {
            // 模拟 Loop-Fiber： 概率 1.0%
            ft = LOOP;
            _R_max = R_max;
            //_R_max = R_loop_max;
            _R_min = R_min;
        } else {
            // 模拟 Migration-Fiber： 剩下的概率
            ft = MIGRATION;
            _R_max = R_max;
            _R_min = R_min;
        }
        GLfloat lastR = _R_min;
        // 对于每一根Fiber， 根据Ply中心计算Fiber位置
        for(int j = 0; j < plyCenter.size(); ++j) {
            // 反求theta
            GLfloat theta = (2.0 * glm::pi<float>() * yarnCenter[j].z) / _alpha;
            // 转换成弧度
            theta = theta * 2.0 * glm::pi<float>() / 360.0f;
            // 获取 R_i 和 theta_i
            GLfloat theta_i = theta_initial[i];
            GLfloat R_i = R_initial[i];
            // 根据 EQ.2 求出 R
            GLfloat R = (R_i / 2.0f) * (_R_max + _R_min + (cos(theta_i + (s * theta)) * (_R_max - _R_min)));
            // 求出 N_ply
            glm::vec3 N_ply = glm::normalize(plyCenter[j] - yarnCenter[j]);
            // glm::vec3 B_ply = derivative(c_ply[j], theta);
            // tricks 求出 双切线 B_ply
            glm::vec3 B_ply = glm::normalize(glm::cross(N_ply, T_yarn));
            // 求出 c_i
            fiberCenter[i][2 * j] = R * ((cos(theta + theta_i) * e_N * N_ply) + (sin(theta + theta_i) * e_B * B_ply));
            fiberCenter[i][2 * j + 1] = glm::normalize(fiberCenter[i][2 * j]);
            // 叠加求出这根 fiber 的位置
            fiberCenter[i][2 * j] = plyCenter[j] + fiberCenter[i][2 * j];
        }
    }
    return fiberCenter;
}

#define DRAW_MODE GL_LINE_STRIP

//
void singleYarn() {
    // 初始化
    srand(time(nullptr));
    GLFWwindow *window = initWindow("TessellationShader", 800, 600, 4, 0);
    showEnviroment();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2.0);
    glLineWidth(1.0);
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
    Shader showNormalShader("shaders/YarnLevelCloth/showNormals.vert", "shaders/YarnLevelCloth/showNormals.frag");
    showNormalShader.addOptionalShader("shaders/YarnLevelCloth/showNormals.geom", GL_GEOMETRY_SHADER);
    Shader bpShader("shaders/YarnLevelCloth/blinnPhong.vert", "shaders/YarnLevelCloth/blinnPhong.frag");
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
    //
    Union *ply1Fibers = new Union();
    vector<vector<glm::vec3>> ply1FiberCenter = calcMigrationFiber(plyCenter1, yarnCenter);
    for(auto f : ply1FiberCenter) {
        Object *fiber = new Object(&f[0].x, f.size() / 2, POSITIONS_NORMALS, DRAW_MODE);
        ply1Fibers->addObject(fiber);
    }
    ply1Fibers->setCamera(cam);
    //
    Union *ply2Fibers = new Union();
    vector<vector<glm::vec3>> ply2FiberCenter = calcMigrationFiber(plyCenter2, yarnCenter);
    for(auto f : ply2FiberCenter) {
        Object *fiber = new Object(&f[0].x, f.size() / 2, POSITIONS_NORMALS, DRAW_MODE);
        ply2Fibers->addObject(fiber);
    }
    ply2Fibers->setCamera(cam);
    //
    Union *ply3Fibers = new Union();
    vector<vector<glm::vec3>> ply3FiberCenter = calcMigrationFiber(plyCenter3, yarnCenter);
    for(auto f : ply3FiberCenter) {
        Object *fiber = new Object(&f[0].x, f.size() / 2, POSITIONS_NORMALS, DRAW_MODE);
        ply3Fibers->addObject(fiber);
    }
    ply3Fibers->setCamera(cam);
    // 可视化横截面的Fiber分布
    vector<GLfloat> R_initial;
    vector<GLfloat> theta_initial;
    int crossSectionFiberNum = 3;
    GLfloat R_now = 0.01;
    for(int k = 0; k < 6; ++k) {
        for(int l = 0; l < crossSectionFiberNum; ++l) {
            theta_initial.push_back(2.0f * glm::pi<float>() / (GLfloat)crossSectionFiberNum * (GLfloat) l);
            R_initial.push_back(R_now);
        }
        R_now += 0.1f;
        crossSectionFiberNum *= 2;
    }
    vector<glm::vec3> crossSectionPoints(R_initial.size());
    for(int i = 0; i < R_initial.size(); ++i) {
        crossSectionPoints[i].x = R_initial[i] * cos(theta_initial[i]);
        crossSectionPoints[i].y = R_initial[i] * sin(theta_initial[i]);
        crossSectionPoints[i].z = 0;
    }
    cout<<"Visualizing a ply cross-section with total "<<crossSectionPoints.size()<<" fibers."<<endl;
    Object *cs = new Object(&crossSectionPoints[0].x, crossSectionPoints.size(), POSITIONS, GL_POINTS);
    cs->setCamera(cam);
    // 主循环
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        shader.use();
        glUniform3f(glGetUniformLocation(bpShader.programID, "light.direction"), -1.0, 0.0, 0.0);
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 1.0, 0.5, 0.5);
        ply1Fibers->setShader(&shader);
        ply1Fibers->draw();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 0.5, 1.0, 0.5);
        ply2Fibers->setShader(&shader);
        ply2Fibers->draw();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 0.5, 0.5, 1.0);
        ply3Fibers->setShader(&shader);
        ply3Fibers->draw();
        /* 指定颜色绘制所有Fibers
        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 1.0, 0.5, 0.5);
        ply1Fibers->setShader(&shader);
        ply1Fibers->draw();
        //
        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 0.5, 1.0, 0.5);
        ply2Fibers->setShader(&shader);
        ply2Fibers->draw();
        //
        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 0.5, 0.5, 1.0);
        ply3Fibers->setShader(&shader);
        ply3Fibers->draw();
        //*/
        /* Debug Draw: 可视化法向量
        ply3Fibers->setShader(&showNormalShader);
        ply3Fibers->draw();
        //*/
        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID, "fragmentColor"), 1.0, 1.0, 1.0);
        cs->setShader(&shader);
        cs->draw();
        //
        panel.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif // YARN_LEVEL_CLOTH_HPP
