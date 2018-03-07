/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef CUBEMAPS_CPP
#define CUBEMAPS_CPP

namespace Cubemapes{

extern GLfloat skyboxVertices[6*6*3];

extern GLfloat cubeVertices[36*5];

extern GLfloat cubeVerticesWithNormals[6*6*6];

void tutorial(){
    // 初始化
    GLFWwindow* window = initWindow("Cubemaps",800,600);
    showEnviroment();
    glfwSwapInterval(0);
    Camera* pCamera = CameraController::getCamera();
    // 设置输入模式
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 绑定输入
    CameraController::bindControl(window);
    // 启用测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // 坐标系对象
    CoordinateAxes ca(pCamera);
    // 立方体纹理
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
        // 天空盒路径
    string cubeTexFilenames[]={"Resources/Textures/skybox/right.jpg","Resources/Textures/skybox/left.jpg","Resources/Textures/skybox/top.jpg",
                                "Resources/Textures/skybox/bottom.jpg","Resources/Textures/skybox/back.jpg","Resources/Textures/skybox/front.jpg"};
        // 因为有正方体贴图包括6张图片，需要导入6个TexImage2D
    for(int i=0;i<6;++i){
        GLuint imageWidth = 0,imageHeight = 0;
        std::unique_ptr<BYTE[]> imageBits = Texture::loadImage(cubeTexFilenames[i].c_str(), imageWidth, imageHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, imageBits.get());
    }
        // 设置贴图参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    // 着色器
    Shader skyboxShader("Resources/Shaders/Cubemaps/cubemap.vs","Resources/Shaders/Cubemaps/cubemap.frag");
    Shader boxShader("Resources/Shaders/Cubemaps/scene.vs","Resources/Shaders/Cubemaps/scene.frag");
    Shader boxReflectShader("Resources/Shaders/Cubemaps/scene_reflect.vs","Resources/Shaders/Cubemaps/scene_reflect.frag");
    Shader boxRefractShader("Resources/Shaders/Cubemaps/scene_refract.vs","Resources/Shaders/Cubemaps/scene_refract.frag");
    // 天空盒物体
    Shape skybox(skyboxVertices, 36, POSITIONS,GL_TRIANGLES);
    // 物体采用反射贴图
    Shape box(cubeVerticesWithNormals, 36, POSITIONS_NORMALS, GL_TRIANGLES);
    // 盒子纹理
    Texture texBox("Resources/Textures/container2.png", GL_BGRA, GL_RGBA);
    // FPS计数器

    // 主循环
    while(!glfwWindowShouldClose(window)){
        // 邻询时间
        glfwPollEvents();
        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // BGC
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        // 更新移动
        CameraController::update();
        // 绘制箱子
        //* 反射
        boxReflectShader.use();
        glUniform3f(glGetUniformLocation(boxReflectShader.programID, "camPosition"),
                    pCamera->cameraPos.x, pCamera->cameraPos.y, pCamera->cameraPos.z);
        box.moveTo(glm::vec3(2.0f, 0.0f, 0.0f));
        box.draw(&boxReflectShader, pCamera);
        //* 折射
        boxRefractShader.use();
        glUniform3f(glGetUniformLocation(boxRefractShader.programID, "camPosition"),
                    pCamera->cameraPos.x, pCamera->cameraPos.y, pCamera->cameraPos.z);
        box.moveTo(glm::vec3(-2.0f, 0.0f, 0.0f));
        box.draw(&boxRefractShader, pCamera);
        // 优化前置深度测试，最后绘制天空盒。其优点在于，通过前置深度测试能使得屏幕上已经被绘制的像素区域天空盒的fragment会被抛弃。
        // 因此天空盒的像素被抛弃的有2部分：1。不通过裁剪测试的，不在视锥体内的vertex；2。通过裁剪测试，但是被物体遮挡的fragment
        glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
        skybox.draw(&skyboxShader, pCamera);
        // 交换双缓冲
        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 练习： 构造含有反射贴图的Model
void exercise1(){
    // 初始化
    GLFWwindow* window=initWindow("Cubemaps-EX1", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    Camera* pCamera = CameraController::getCamera();
    // 设置输入模式
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 绑定输入
    CameraController::bindControl(window);
    // 启用测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // 坐标系对象
    CoordinateAxes ca(pCamera);
    // 立方体纹理
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
        // 天空盒路径
    string cubeTexFilenames[] = {"Resources/Textures/skybox/right.jpg","Resources/Textures/skybox/left.jpg","Resources/Textures/skybox/top.jpg",
                                "Resources/Textures/skybox/bottom.jpg","Resources/Textures/skybox/back.jpg","Resources/Textures/skybox/front.jpg"};
        // 因为有正方体贴图包括6张图片，需要导入6个TexImage2D
    for(int i = 0; i < 6; ++i){
        GLuint imageWidth = 0,imageHeight = 0;
        std::unique_ptr<BYTE[]> imageBits = Texture::loadImage(cubeTexFilenames[i].c_str(), imageWidth, imageHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, imageBits.get());
    }
        // 设置贴图参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    // 天空盒物体
        // 着色器
    Shader skyboxShader("Resources/Shaders/Cubemaps/cubemap.vs","Resources/Shaders/Cubemaps/cubemap.frag");
    Shape skybox(skyboxVertices,36,POSITIONS,GL_TRIANGLES);
    //导入模型
        // 着色器
    Shader modelShader("Resources/Shaders/Cubemaps/nanosuit_reflect.vs","Resources/Shaders/Cubemaps/nanosuit_reflect.frag");
    Model model((GLchar*)"Resources/Meshes/nanosuit_reflection/nanosuit.obj");
    // FPS计数器

    // 主循环
    while(!glfwWindowShouldClose(window)){
        // 邻询时间
        glfwPollEvents();
        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // BGC
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        // 更新移动
        CameraController::update();
        //
        modelShader.use();
        glUniform3f(glGetUniformLocation(modelShader.programID, "fCameraPos"),
                    pCamera->cameraPos.x, pCamera->cameraPos.y, pCamera->cameraPos.z);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
        glUniform1i(glGetUniformLocation(modelShader.programID, "texture_cube1"), 4);
        model.draw(&modelShader, pCamera);
        // 绘制坐标轴
        ca.draw();

        // 优化前置深度测试，最后绘制天空盒。其优点在于，通过前置深度测试能使得屏幕上已经被绘制的像素区域天空盒的fragment会被抛弃。
        // 因此天空盒的像素被抛弃的有2部分：1。不通过裁剪测试的，不在视锥体内的vertex；2。通过裁剪测试，但是被物体遮挡的fragment
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
        skybox.draw(&skyboxShader, pCamera);

        // 交换双缓冲
        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
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

GLfloat cubeVerticesWithNormals[6*6*6] = {
    // Positions          // Normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
GLfloat skyboxVertices[] = {
    // Positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
};
#endif // CUBEMAPS_HPP
