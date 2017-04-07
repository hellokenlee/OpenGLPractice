/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef ANTI_ALIASING_HPP
#define ANTI_ALIASING_HPP
namespace AntiAliasing{
extern GLfloat cubeVertices[6*6*5];

// 使用GLFW提供的MSAA缓冲
void tutorial(){
    // 初始化
    //初始化GLFW。设置GLFW参数，GL版本3.3，只使用CORE_PROFILE，不能Resize
    glfwInit();
    // 设置多重采样样本缓冲为4x
    glfwWindowHint(GLFW_SAMPLES, 32);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    //创建窗口
    int w=800,h=600;
    GLFWwindow *window = glfwCreateWindow(w, h, "AntiAliasing",nullptr,nullptr);
    //检查是否成功
    if(window == nullptr){
        cout<<"ERROR: Fail to create GLFW window"<<endl;
        glfwTerminate();
        exit(-1);
    }
    //获取显示器大小
    const GLFWvidmode *screen;
    screen=glfwGetVideoMode(glfwGetPrimaryMonitor());
    //设置窗口在中间
    glfwSetWindowPos(window,(screen->width-w)/2,(screen->height-h)/2);
    glfwMakeContextCurrent(window);
    //初始化GLEW。 使用Experimental函数允许我们更好的使用core_profile
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){
        cout<<"ERROR: Fail to init GLEW"<<endl;
        exit(-1);
    }
    glGetError();//忽略由glew引起的INVALID_ENUM错误
    //获取当前窗口大小，设置GLVIewport
    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);
    showEnviroment();
    glfwSwapInterval(0);
    // 设置输入模式
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 绑定输入
    //CameraController::bindControl(window);
    CameraController::moveSpeed = 0.5f;
    // 启用测试
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //glDisable(GL_MULTISAMPLE);
    // 坐标系对象
    CoordinateAxes ca(&CameraController::camera);
    // 着色器
    Shader cubeShader("shaders/AntiAliasing/object.vs", "shaders/AntiAliasing/object.frag");
    // 方块
    Object cube(cubeVertices, 36, POSITIONS_TEXTURES, GL_TRIANGLES);
    cube.setShader(&cubeShader);
    cube.setCamera(&CameraController::camera);
    CameraController::camera.cameraPos = glm::vec3(-0.177235f, 0.197711f, 1.68138f);
    CameraController::camera.rotate(3.14999,-78.5998);
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        cube.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}


extern GLfloat screenVertices[6*5];
// 使用FB自己创建离屏MSAA渲染
void exercise(){
    GLFWwindow* window = initWindow("AA", 800, 600);
    glfwSwapInterval(0);
    // 设置输入模式
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 绑定输入
    CameraController::bindControl(window);
    CameraController::moveSpeed = 0.5f;
    // 启用测试
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    // 着色器
    Shader cubeShader("shaders/AntiAliasing/object.vs", "shaders/AntiAliasing/object.frag");
    Shader screenShader("shaders/AntiAliasing/screen.vs", "shaders/AntiAliasing/screen.frag");
    // 方块
    Object cube(cubeVertices, 36, POSITIONS_TEXTURES, GL_TRIANGLES);
    cube.setShader(&cubeShader);
    cube.setCamera(&CameraController::camera);
    Object screen(screenVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    screen.setShader(&screenShader);
    screen.setCamera(&CameraController::camera);
    // 创建离屏缓冲
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        // 创建多重采样纹理
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 1, GL_RGB, 800, 600, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        // 附加到FB上
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
        // 生产Render Buffer Object
        GLuint rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 1, GL_DEPTH24_STENCIL8, 800, 600);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        // 附加到FB上
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        //错误检查
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
            cout<<"ERROR:: FRAMEBUFFER init faild!"<<endl;
            return;
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();


        // 在FB上绘制
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.draw();

        // 转移结果到默认FBO
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        checkError();

        // 显示
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

}
#endif // ANTI_ALIASING_HPP
