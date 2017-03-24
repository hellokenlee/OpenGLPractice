/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef FRAMEBUFFERS_HPP
#define FRAMEBUFFERS_HPP
namespace Framebuffers{
//顶点信息前置声明
extern GLfloat cubeVertices[36*5];
extern GLfloat planeVertices[6*5];
extern GLfloat screenVertices[6*5];
//物体位置信息
glm::vec3 cubePositions[2]={glm::vec3(-1.0f, 0.01f, -1.0f),glm::vec3(2.0f, 0.01f, 0.0f)};

//教程实现
void tutorial(){
    // Set up window
    GLFWwindow* window=initWindow("Framebuffer",800,600);
    showEnviroment();
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    // Set up coordinate axes
    CoordinateAxes ca(&CameraController::camera);

    //创建帧缓冲对象
    GLuint framebuffer;
    glGenFramebuffers(1,&framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        //创建纹理缓冲附件对象
        GLuint texColorBuffer;
        glGenTextures(1,&texColorBuffer);
        glBindTexture(GL_TEXTURE_2D,texColorBuffer);
                        //种类       level  纹理格式  宽x高   边界  图片格式  数据格式    数据指针
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D,0);
        //附着到帧缓冲对象上
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texColorBuffer,0);
        //创建深度测试和模板测试缓冲附件，以便于GL在绘制该FB的时候可以进行深度测试和模板测试
        GLuint renderBuffer;
        glGenRenderbuffers(1,&renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer);
            //分配内存
            glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,800,600);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
        //附着到帧缓冲对象上
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
        //错误检查
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
            cout<<"ERROR:: FRAMEBUFFER init faild!"<<endl;
            return;
        }
    glBindFramebuffer(GL_FRAMEBUFFER,0);


    //场景初始化
    // Setup and compile our shaders
    Shader shader("shaders/Framebuffers/scene1.vs", "shaders/Framebuffers/scene1.frag");
    Shader shader2("shaders/Framebuffers/2D_With_Texture.vs", "shaders/Framebuffers/2D_With_Texture_Filtered3x3.frag");
    // Setup cube VAO
    Object cube(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&shader);
    // Setup plane VAO
    Object plane(planeVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    plane.setCamera(&CameraController::camera);
    plane.setShader(&shader);
    //
    Object screen(screenVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    screen.setShader(&shader2);
    // Load textures
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container.jpg",0,GL_BGR,GL_RGB))
        return ;
    if(!tm->loadTexture("textures/wall.jpg",1,GL_BGR,GL_RGB))
        return ;

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        //第一次绘制：生成场景 生成纹理
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        CameraController::update();
        tm->bindTexture(0);
        cube.moveTo(cubePositions[0]);
        cube.draw();
        cube.moveTo(cubePositions[1]);
        cube.draw();
        tm->bindTexture(1);
        plane.draw();
        //第二次绘制，把生成的纹理绘制到屏幕四边形上
        //绑定回默认FB

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D,texColorBuffer);
        screen.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();

}

GLfloat mirrorVertices[6*5]={
    // Positions         // TexCoords
     0.7f,  1.0f, 0.0f,  0.0f, 1.0f,
     0.7f,  0.7f, 0.0f,  0.0f, 0.0f,
     1.0f,  0.7f, 0.0f,  1.0f, 0.0f,

     0.7f,  1.0f, 0.0f,  0.0f, 1.0f,
     1.0f,  0.7f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

//倒后镜效果
void exercise1(){
    // Set up window
    GLFWwindow* window=initWindow("Framebuffer",800,600);
    showEnviroment();
    glfwSwapInterval(0);
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    // Set up coordinate axes
    CoordinateAxes ca(&CameraController::camera);

    //创建帧缓冲对象
    GLuint framebuffer;
    glGenFramebuffers(1,&framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        //创建纹理缓冲附件对象
        GLuint texColorBuffer;
        glGenTextures(1,&texColorBuffer);
        glBindTexture(GL_TEXTURE_2D,texColorBuffer);
                        //种类       level  纹理格式  宽x高   边界  图片格式  数据格式    数据指针
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D,0);
        //附着到帧缓冲对象上
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texColorBuffer,0);
        //创建深度测试和模板测试缓冲附件，以便于GL在绘制该FB的时候可以进行深度测试和模板测试
        GLuint renderBuffer;
        glGenRenderbuffers(1,&renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer);
            //分配内存
            glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,800,600);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
        //附着到帧缓冲对象上
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
        //错误检查
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
            cout<<"ERROR:: FRAMEBUFFER init faild!"<<endl;
            return;
        }
    glBindFramebuffer(GL_FRAMEBUFFER,0);


    //场景初始化
    // Setup and compile our shaders
    Shader shader("shaders/Framebuffers/scene1.vs", "shaders/Framebuffers/scene1.frag");
    Shader shader2("shaders/Framebuffers/2D_With_Texture.vs", "shaders/Framebuffers/2D_With_Texture.frag");
    // Setup cube VAO
    Object cube(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&shader);
    // Setup plane VAO
    Object plane(planeVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    plane.setCamera(&CameraController::camera);
    plane.setShader(&shader);
    //
    Object screen(screenVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    screen.setShader(&shader2);

    Object mirror(mirrorVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    mirror.setShader(&shader2);
    //
    // Load textures
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container.jpg",0,GL_BGR,GL_RGB))
        return ;
    if(!tm->loadTexture("textures/wall.jpg",1,GL_BGR,GL_RGB))
        return ;

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        //第一次绘制：生成场景 镜子场景
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        CameraController::camera.rotate(CameraController::pitch-180,CameraController::yaw);
        tm->bindTexture(0);
        cube.moveTo(cubePositions[0]);
        cube.draw();
        cube.moveTo(cubePositions[1]);
        cube.draw();
        tm->bindTexture(1);
        plane.draw();
        CameraController::camera.rotate(CameraController::pitch,CameraController::yaw);


        //第二次绘制，绘制场景+镜子
        //绑定回默认FB
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        tm->bindTexture(0);
        cube.moveTo(cubePositions[0]);
        cube.draw();
        cube.moveTo(cubePositions[1]);
        cube.draw();
        tm->bindTexture(1);
        plane.draw();
        glBindTexture(GL_TEXTURE_2D,texColorBuffer);
        mirror.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();

}


GLfloat screenVertices[6*5]={
    // Positions         // TexCoords
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};
// Set the object data (buffers, vertex attributes)
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
#endif
