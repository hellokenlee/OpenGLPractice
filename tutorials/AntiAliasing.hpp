/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef ANTI_ALIASING_CPP
#define ANTI_ALIASING_CPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

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
    screen = glfwGetVideoMode(glfwGetPrimaryMonitor());
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
    CoordinateAxes ca(CameraController::getCamera());
    // 着色器
    Shader cubeShader("Resources/Shaders/AntiAliasing/object.vs", "Resources/Shaders/AntiAliasing/object.frag");
    // 方块
    Shape cube(cubeVertices, 36, POSITIONS_TEXTURES, GL_TRIANGLES);
    CameraController::getCamera()->cameraPos = glm::vec3(-0.177235f, 0.197711f, 1.68138f);
    CameraController::getCamera()->rotate(3.14999,-78.5998);
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        cube.draw(&cubeShader, CameraController::getCamera());

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}


extern GLfloat screenVertices[6*5];
// 使用FB自己创建离屏MSAA渲染
void exercise(){
    GLFWwindow* window = initWindow("AA", 800, 600);
    showEnviroment();
    // 设置输入模式
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 绑定输入
    CameraController::bindControl(window);
    CameraController::moveSpeed = 0.5f;
    // 启用测试
    glEnable(GL_DEPTH_TEST);
    // 着色器
    Shader cubeShader("Resources/Shaders/AntiAliasing/object.vs", "Resources/Shaders/AntiAliasing/object.frag");
    Shader screenShader("Resources/Shaders/AntiAliasing/screen.vs", "Resources/Shaders/AntiAliasing/screen.frag");
    // 方块
    Shape cube(cubeVertices, 36, POSITIONS_TEXTURES, GL_TRIANGLES);
    Shape screen(screenVertices,6,POSITIONS_TEXTURES,GL_TRIANGLES);
    // 创建离屏缓冲
    GLuint MSAAfbo;
    glGenFramebuffers(1, &MSAAfbo);
    glBindFramebuffer(GL_FRAMEBUFFER, MSAAfbo);
        // 创建多重采样纹理
        GLuint MSAAtex;
        glGenTextures(1, &MSAAtex);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MSAAtex);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 1, GL_RGB, 800, 600, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        // 附加到FB上
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, MSAAtex, 0);
        // 生产Render Buffer Shape
        GLuint MSAArbo;
        glGenRenderbuffers(1, &MSAArbo);
        glBindRenderbuffer(GL_RENDERBUFFER, MSAArbo);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 1, GL_DEPTH24_STENCIL8, 800, 600);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        // 附加到FB上
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, MSAArbo);
        //错误检查
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
            cout<<"ERROR:: FRAMEBUFFER init faild!"<<endl;
            return;
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //创建帧缓冲对象
    GLuint fbo;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
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

    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        // 在MSAAFB上绘制
        glBindFramebuffer(GL_FRAMEBUFFER, MSAAfbo);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.draw(&cubeShader, CameraController::getCamera());

        // 转移结果到中间
        glBindFramebuffer(GL_READ_FRAMEBUFFER, MSAAfbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
        glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        /*
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        checkError();
        */
        // 在默认FB绘制
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        screen.draw(&screenShader);

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
