/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef BASIC_LIGHTING_CPP
#define BASIC_LIGHTING_CPP

namespace BasicLighting{
//顶点信息前置声明
extern GLfloat cubeVertices[36*6];
//一些光照设置
float cubeColor[]={1.0f, 0.5f, 0.31f};
float lampColor[]={1.0f,1.0f,1.0f};
glm::vec3 cubePos(0.0f,0.5f,0.0f);
glm::vec3 lampPos(2.5f,2.0f,0.0f);

//教程实现：实现phong光照模型(环境光+漫反射光+镜面反射光)
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("BasicLighting",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //着色器程序初始化
    Shader cubeShader("Resources/Shaders/BasicLighting/cube.vs","Resources/Shaders/BasicLighting/cube.frag");
    Shader lampShader("Resources/Shaders/BasicLighting/lamp.vs","Resources/Shaders/BasicLighting/lamp.frag");
    cubeShader.use();
    glUniform3f(glGetUniformLocation(cubeShader.programID,"objColor"),cubeColor[0],cubeColor[1],cubeColor[2]);
    glUniform3f(glGetUniformLocation(cubeShader.programID,"lightColor"),lampColor[0],lampColor[1],lampColor[2]);
    glUniform3f(glGetUniformLocation(cubeShader.programID,"lightPos"),lampPos[0],lampPos[1],lampPos[2]);


    GLuint viewPosLoc=glGetUniformLocation(cubeShader.programID,"viewPos");
    lampShader.use();
    glUniform3f(glGetUniformLocation(lampShader.programID,"lampColor"),lampColor[0],lampColor[1],lampColor[2]);
    //物体对象初始化
    Shape cube(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    cube.moveTo(cubePos);

    Shape lamp(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    lamp.moveTo(lampPos);
    lamp.scaleTo(glm::vec3(0.4));
    //绑定控制
    glfwSetKeyCallback(window,CameraController::keyCallback);
    glfwSetCursorPosCallback(window,CameraController::mouseCallback);
    glfwSetScrollCallback(window,CameraController::scrollCallback);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    //帧数计数器

    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        ca.draw();
        lamp.draw(&lampShader, pCamera);

        cubeShader.use();
        glUniform3fv(viewPosLoc, 1, pCamera->getPositionVal());
        cube.draw(&cubeShader, pCamera);

        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

//练习1： 光源随时间移动
void exercise1(){
    //创建窗口
    GLFWwindow *window=initWindow("BasicLighting",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //着色器程序初始化
    Shader cubeShader("Resources/Shaders/BasicLighting/cube.vs","Resources/Shaders/BasicLighting/cube.frag");
    Shader lampShader("Resources/Shaders/BasicLighting/lamp.vs","Resources/Shaders/BasicLighting/lamp.frag");
    cubeShader.use();
    glUniform3f(glGetUniformLocation(cubeShader.programID,"objColor"),cubeColor[0],cubeColor[1],cubeColor[2]);
    glUniform3f(glGetUniformLocation(cubeShader.programID,"lightColor"),lampColor[0],lampColor[1],lampColor[2]);

    GLuint lightPosLoc=glGetUniformLocation(cubeShader.programID,"lightPos");
    GLuint viewPosLoc=glGetUniformLocation(cubeShader.programID,"viewPos");
    lampShader.use();
    glUniform3f(glGetUniformLocation(lampShader.programID,"lampColor"),lampColor[0],lampColor[1],lampColor[2]);
    //物体对象初始化
    Shape cube(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    cube.moveTo(cubePos);

    Shape lamp(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    lamp.moveTo(lampPos);
    lamp.scaleTo(glm::vec3(0.4f));
    //绑定控制
    glfwSetKeyCallback(window,CameraController::keyCallback);
    glfwSetCursorPosCallback(window,CameraController::mouseCallback);
    glfwSetScrollCallback(window,CameraController::scrollCallback);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    //帧数计数器

    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        //更新光源z坐标位置
        glm::vec3 lightPos=lampPos;
        lightPos.z+=sin(glfwGetTime())*3;
        lamp.moveTo(lightPos);
        lamp.draw(&lampShader, pCamera);

        cubeShader.use();
        glUniform3fv(viewPosLoc, 1, pCamera->getPositionVal());
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        cube.draw(&cubeShader, pCamera);

        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

//练习2只是改变发光值(Shininess),因此没有再写一个函数
//练习3： 在viewspace中计算光照:注意 法线的更新计算
void exercise3(){
    //创建窗口
    GLFWwindow *window=initWindow("BasicLighting",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //着色器程序初始化
    Shader cubeShader("Resources/Shaders/BasicLighting/cubeEx3.vs","Resources/Shaders/BasicLighting/cubeEx3.frag");
    Shader lampShader("Resources/Shaders/BasicLighting/lamp.vs","Resources/Shaders/BasicLighting/lamp.frag");
    cubeShader.use();
    glUniform3f(glGetUniformLocation(cubeShader.programID,"objColor"),cubeColor[0],cubeColor[1],cubeColor[2]);
    glUniform3f(glGetUniformLocation(cubeShader.programID,"lightColor"),lampColor[0],lampColor[1],lampColor[2]);
    glUniform3f(glGetUniformLocation(cubeShader.programID,"LightPos"),lampPos[0],lampPos[1],lampPos[2]);
    lampShader.use();
    glUniform3f(glGetUniformLocation(lampShader.programID,"lampColor"),lampColor[0],lampColor[1],lampColor[2]);
    //物体对象初始化
    Shape cube(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    cube.moveTo(cubePos);

    Shape lamp(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    lamp.moveTo(lampPos);
    lamp.scaleTo(glm::vec3(0.4f));
    //绑定控制
    glfwSetKeyCallback(window,CameraController::keyCallback);
    glfwSetCursorPosCallback(window,CameraController::mouseCallback);
    glfwSetScrollCallback(window,CameraController::scrollCallback);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    //帧数计数器

    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lamp.draw(&lampShader, pCamera);

        cube.draw(&cubeShader, pCamera);

        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

//练习4： 实现 Gouraud 光照，即在vertexShader中实现光照计算，对像素插值处理
void exercise4(){
    //创建窗口
    GLFWwindow *window=initWindow("BasicLighting",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //着色器程序初始化
    Shader cubeShader("Resources/Shaders/BasicLighting/cubeEx4.vs","Resources/Shaders/BasicLighting/cubeEx4.frag");
    Shader lampShader("Resources/Shaders/BasicLighting/lamp.vs","Resources/Shaders/BasicLighting/lamp.frag");
    cubeShader.use();
    glUniform3f(glGetUniformLocation(cubeShader.programID,"objColor"),cubeColor[0],cubeColor[1],cubeColor[2]);
    glUniform3f(glGetUniformLocation(cubeShader.programID,"lightColor"),lampColor[0],lampColor[1],lampColor[2]);
    glUniform3f(glGetUniformLocation(cubeShader.programID,"LightPos"),lampPos[0],lampPos[1],lampPos[2]);
    lampShader.use();
    glUniform3f(glGetUniformLocation(lampShader.programID,"lampColor"),lampColor[0],lampColor[1],lampColor[2]);
    //物体对象初始化
    Shape cube(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    cube.moveTo(cubePos);

    Shape lamp(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    lamp.moveTo(lampPos);
    lamp.scaleTo(glm::vec3(0.4f));
    //绑定控制
    glfwSetKeyCallback(window,CameraController::keyCallback);
    glfwSetCursorPosCallback(window,CameraController::mouseCallback);
    glfwSetScrollCallback(window,CameraController::scrollCallback);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    //帧数计数器

    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lamp.draw(&lampShader, pCamera);

        cube.draw(&cubeShader, pCamera);

        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
//顶点信息
GLfloat cubeVertices[36*6] = {
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
};
#endif  //  BASIC_LIGHTING
