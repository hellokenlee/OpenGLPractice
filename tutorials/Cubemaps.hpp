/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef CUBEMAPS_HPP
#define CUBEMAPS_HPP
namespace Cubemapes{

extern GLfloat skyboxVertices[6*6*3];

void tutorial(){
    // 初始化
    GLFWwindow* window=initWindow("Cubemaps",800,600);
    showEnviroment();
    // 设置输入模式
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 绑定输入
    CameraController::bindControl(window);
    // 启用测试
    glEnable(GL_DEPTH_TEST);
    // 坐标系对象
    CoordinateAxes ca(&CameraController::camera);
    // 立方体纹理
    GLuint texID;
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP,texID);
        // 天空盒路径
    string cubeTexFilenames[]={"textures/skybox/right.jpg","textures/skybox/left.jpg","textures/skybox/top.jpg",
                                "textures/skybox/bottom.jpg","textures/skybox/back.jpg","textures/skybox/front.jpg"};
    TextureManager* tm=TextureManager::getManager();
        // 因为有正方体贴图包括6张图片，需要导入6个TexImage2D
    for(int i=0;i<6;++i){
        GLuint imageWidth=0,imageHeight=0;
        BYTE* imageBits=tm->loadImage(cubeTexFilenames[i].c_str(),imageWidth,imageHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, imageBits);
    }
        // 设置贴图参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    // 着色器
    Shader shader("shaders/Cubemaps/cubemap.vs","shaders/Cubemaps/cubemap.frag");
    // 天空盒物体
    Object skybox(skyboxVertices,36,POSITIONS,GL_TRIANGLES);
    skybox.setCamera(&CameraController::camera);
    skybox.setShader(&shader);
    skybox.scaleTo(10.0f);
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

        glBindTexture(GL_TEXTURE_CUBE_MAP,texID);
        skybox.draw();

        ca.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

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
