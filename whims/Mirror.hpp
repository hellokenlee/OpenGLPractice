/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MIRROR_HPP
#define MIRROR_HPP

using namespace std;

namespace Mirror {

// 顶点信息前置声明
extern GLfloat cubeVertices[36*5];
extern GLfloat planeVertices[6*5];
extern GLfloat mirrorVertices[6*5];

// 箱子位置
glm::vec3 cubePositions[2] = {glm::vec3(-1.0f, 0.01f, -1.0f), glm::vec3(2.0f, 0.01f, 0.0f)};
// 镜子位置
glm::vec3 mirrorPosition = glm::vec3(0.0f, 0.6f, -5.0f);
// 镜子法向量
glm::vec3 mirrorNormal = glm::vec3(0.0f, 0.0f, 1.0f);

//突发奇想想做个镜子
void _main() {
    // Init GLFW
    GLFWwindow* window=initWindow("Mirror",800,600);
    showEnviroment();
    glfwSwapInterval(0);
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
    Shader shader("shaders/Mirror/scene.vs", "shaders/Mirror/scene.frag");
    Shader mirrorShader("shaders/Mirror/scene_mirrored.vs", "shaders/Mirror/scene_mirrored.frag");
    // 写入镜子的顶点信息
    mirrorShader.use();
    glUniform3f(glGetUniformLocation(mirrorShader.programID,"mirror_position"),mirrorPosition.x,mirrorPosition.y,mirrorPosition.z);
    glUniform3f(glGetUniformLocation(mirrorShader.programID,"mirror_normal"),mirrorNormal.x,mirrorNormal.y,mirrorNormal.z);
    // 初始化方块物体
    Object cube(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&shader);
    // 初始化地板物体
    Object plane(planeVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    plane.setCamera(&CameraController::camera);
    plane.setShader(&shader);
    // 初始化镜子物体
    Object mirror(mirrorVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    mirror.setCamera(&CameraController::camera);
    mirror.setShader(&shader);
    mirror.moveTo(mirrorPosition);
    FPSCounter fc;
    // 读入纹理
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container2.png",0,GL_BGRA,GL_RGBA))
        return ;
    if(!tm->loadTexture("textures/wall.jpg",1,GL_BGR,GL_RGB))
        return ;
    if(!tm->loadTexture("textures/container2_specular.png",2,GL_BGRA,GL_RGBA))
        return ;
    // 显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        //开启模板缓冲写，不然clear就没作用了。
        glStencilMask(0xFF);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        glStencilMask(0x00);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        //第一次绘制，绘制正常世界
            //关闭模板缓冲写
        glStencilMask(0x00);
            //重设模板函数
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        tm->bindTexture(0);
        cube.setShader(&shader);
        cube.moveTo(cubePositions[0]);
        cube.draw();
        cube.moveTo(cubePositions[1]);
        cube.draw();
        tm->bindTexture(1);
        plane.setShader(&shader);
        plane.draw();
        //绘制镜子
            //开启模板缓冲写,另镜子中的模板缓冲为全1
        glStencilMask(0xFF);
        glDepthMask(0x00);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glBindTexture(GL_TEXTURE_2D,0);
        mirror.scaleTo(1.0f);
        mirror.draw();

        //绘制镜子边框，放大1.1倍，类似于物体边缘绘制法
        glStencilMask(0x00);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        tm->bindTexture(2);
        mirror.scaleTo(1.1f);
        mirror.draw();

        //第二次绘制，绘制镜子里世界
            //关闭模板缓冲写
        glStencilMask(0x00);
        glDepthMask(0xFF);
            //清空深度缓冲
        glClear(GL_DEPTH_BUFFER_BIT);
            //只有模板为1的时候才绘制
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        tm->bindTexture(0);
        cube.setShader(&mirrorShader);
        cube.moveTo(cubePositions[0]);
        cube.draw();
        cube.moveTo(cubePositions[1]);
        cube.draw();
        tm->bindTexture(1);
        plane.setShader(&mirrorShader);
        plane.draw();

        glfwSwapBuffers(window);
        fc.update();
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
