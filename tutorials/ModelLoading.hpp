/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MODEL_LOADING_HPP
#define MODEL_LOADING_HPP
namespace ModelLoading{
//顶点信息前置声明
extern GLfloat cubeVertices[36*8];
//点灯源位置
glm::vec3 lightPos(0.2f, 10.0f, 1.5f);
//光照颜色(光源颜色)
glm::vec3 lightColor(1.0f,1.0f,1.0f);

//显示程序
void tutorial(){
    GLFWwindow *window=initWindow("ModelLoading",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);
    //着色器初始化
    Shader modelShader("shaders/ModelLoading/nanosuit.vs","shaders/ModelLoading/nanosuit.frag");
    //模型初始化
    Model model("textures/nanosuit/nanosuit.obj");
    model.setShader(&modelShader);
    model.setCamera(&CameraController::camera);
    //绑定控制
    CameraController::bindControl(window);
    CameraController::camera.moveto(glm::vec3(0,20,20));
    CameraController::camera.rotate(-40,0);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //绘制
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        CameraController::update();

        ca.draw();
        model.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void exercise1(){
    GLFWwindow *window=initWindow("ModelLoading-Ex2",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);

    //
    Shader lampShader("shaders/ModelLoading/lamp.vs","shaders/ModelLoading/lamp.frag");
    lampShader.use();
    glUniform3f(glGetUniformLocation(lampShader.programID,"lampColor"),lightColor.x,lightColor.y,lightColor.z);
    //
    Object lamp(cubeVertices,36,POSITIONS_NORMALS_TEXTURES,GL_TRIANGLES);
    lamp.setCamera(&CameraController::camera);
    lamp.setShader(&lampShader);
    lamp.scaleTo(0.1);
    lamp.moveTo(lightPos);
    //着色器初始化
    Shader modelShader("shaders/ModelLoading/nanosuit2.vs","shaders/ModelLoading/nanosuit2.frag");
    //设置灯光参数
    modelShader.use();
    glm::vec3 lightDiffuseColor = lightColor * glm::vec3(0.7f); // Decrease the influence
    glm::vec3 lightAmbientColor = lightColor * glm::vec3(0.3f); // Low influence
    glm::vec3 lightSpecularColor = lightColor * glm::vec3(1.0f); // Low influence
    glUniform3f(glGetUniformLocation(modelShader.programID,"vLight.position"),lightPos.x,lightPos.y,lightPos.z);
    glUniform3f(glGetUniformLocation(modelShader.programID,"vLight.ambient"),lightAmbientColor.x,lightAmbientColor.y,lightAmbientColor.z);
    glUniform3f(glGetUniformLocation(modelShader.programID,"vLight.diffuse"),lightDiffuseColor.x,lightDiffuseColor.y,lightDiffuseColor.z);
    glUniform3f(glGetUniformLocation(modelShader.programID,"vLight.specular"),lightSpecularColor.x,lightSpecularColor.y,lightSpecularColor.z);
    glUniform1f(glGetUniformLocation(modelShader.programID,"shininess"),32.0f);
    //模型初始化
    Model model("textures/nanosuit/nanosuit.obj");
    model.setShader(&modelShader);
    model.setCamera(&CameraController::camera);
    //绑定控制
    CameraController::bindControl(window);
    CameraController::camera.moveto(glm::vec3(0,20,20));
    CameraController::camera.rotate(-40,0);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //绘制
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        CameraController::update();

        lamp.draw();
        ca.draw();
        model.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
//顶点信息
GLfloat cubeVertices[36*8] = {
    // Positions           // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
};
#endif // MODEL_LOADING_HPP
