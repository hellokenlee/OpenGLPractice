/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MIRROR_HPP
#define MIRROR_HPP

namespace Mirror {

// 顶点信息前置声明
extern GLfloat cubeVertices[36*5];
extern GLfloat planeVertices[6*5];
extern GLfloat mirrorVertices[6*5];

// 箱子位置
glm::vec3 cubePositions[2] = {glm::vec3(-1.0f, 0.01f, -1.0f), glm::vec3(2.0f, 0.01f, 0.0f)};
// 镜子位置
glm::vec3 mirrorPosition = glm::vec3(0.0f, 0.5f, -5.0f);
// 镜子法向量
glm::vec3 mirrorNormal = glm::vec3(0.0f, 0.0f, 1.0f);

//突发奇想想做个镜子
void _main() {
    // Init GLFW
    GLFWwindow* window=initWindow("Mirror",800,600);
    showEnviroment();
    glfwSwapInterval(0);
    Camera *pCamera = CameraController::getCamera();
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    // 模板测试失败时：保持现有模板值；模板测试成功但深度测试失败：保持现有模板值；两个都成功： 替换成测试的参考值。
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // 初始化着色器
    Shader sceneShader("Resources/Shaders/Mirror/scene.vs", "Resources/Shaders/Mirror/scene.frag");
    Shader mirrorShader("Resources/Shaders/Mirror/scene_mirrored.vs", "Resources/Shaders/Mirror/scene_mirrored.frag");
    // 写入镜子的顶点信息
    mirrorShader.use();
    glUniform3f(glGetUniformLocation(mirrorShader.programID,"mirror_position"),mirrorPosition.x,mirrorPosition.y,mirrorPosition.z);
    glUniform3f(glGetUniformLocation(mirrorShader.programID,"mirror_normal"),mirrorNormal.x,mirrorNormal.y,mirrorNormal.z);
    // 初始化方块物体
    Shape cube(cubeVertices, 36, POSITIONS_TEXTURES,GL_TRIANGLES);
    // 初始化地板物体
    Shape plane(planeVertices, 6, POSITIONS_TEXTURES,GL_TRIANGLES);
    // 初始化镜子物体
    Shape mirror(mirrorVertices, 6, POSITIONS_TEXTURES,GL_TRIANGLES);
    mirror.moveTo(mirrorPosition);

    // 读入纹理
    Texture tex0("Resources/Textures/container2.png", GL_BGRA, GL_RGBA);
    Texture tex1("Resources/Textures/wall.jpg", GL_BGR, GL_RGB);
    Texture tex2("Resources/Textures/container2_specular.png", GL_BGRA, GL_RGBA);
    // 显示坐标轴
    CoordinateAxes ca(pCamera);
    glm::vec3 BGColor(0.2f, 0.2f, 0.2f);
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        // 开启模板缓冲写，不然clear就没作用了。
        glStencilMask(0xFF);
        glClearColor(BGColor.r, BGColor.g, BGColor.b, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // 绘制镜子模板区域
        glStencilMask(0xFF);// 开启模板缓冲写 镜子中的模板缓冲为全1
        glDepthMask(0x00);// 不写入深度
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glBindTexture(GL_TEXTURE_2D, 0);
        mirror.scaleTo(1.0f);
        mirror.draw(&sceneShader, pCamera);
        // 镜子里世界
        glStencilMask(0x00);// 关闭模板缓冲写
        glDepthMask(0xFF);
        glStencilFunc(GL_EQUAL, 1, 0xFF);// 只有模板为1的时候才绘制
        glClearColor(BGColor.r, BGColor.g, BGColor.b, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        tex0.use();
        cube.moveTo(cubePositions[0]);
        cube.draw(&mirrorShader, pCamera);
        cube.moveTo(cubePositions[1]);
        cube.draw(&mirrorShader, pCamera);
        tex1.use();
        plane.draw(&mirrorShader, pCamera);
        //绘制正常世界
        glStencilMask(0x00);// 关闭模板缓冲写
        glStencilFunc(GL_ALWAYS, 1, 0xFF);// 重设模板函数
        tex0.use();
        cube.moveTo(cubePositions[0]);
        cube.draw(&sceneShader, pCamera);
        cube.moveTo(cubePositions[1]);
        cube.draw(&sceneShader, pCamera);
        tex1.use();
        plane.draw(&sceneShader, pCamera);
        // 绘制镜子边框，放大1.1倍，类似于物体边缘绘制法
        glStencilMask(0x00);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        tex2.use();
        mirror.scaleTo(1.1f);
        mirror.draw(&sceneShader, pCamera);
        //
        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}


GLfloat mirrorVertices[6*5]={
    // Positions         // TexCoords
     0.5f, -1.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  1.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -1.0f,  0.0f,  0.0f, 1.0f
};
GLfloat cubeVertices[6*6*5] = {
    // Positions          // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
GLfloat planeVertices[6*5] = {
    // Positions            // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
    5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
};

};
#endif // MIRROR_HPP
