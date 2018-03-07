/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MODEL_LOADING_CPP
#define MODEL_LOADING_CPP

namespace ModelLoading{
//顶点信息前置声明
extern GLfloat cubeVertices[36*8];
#define NR_POINT_LIGHTS 3
//点灯源位置
glm::vec3 lightPos[7] = {
    glm::vec3(0.0f, 16.0f, 1.5f),
    glm::vec3(-3.0f, 12.0f, 1.5f),
    glm::vec3(3.0f, 12.0f, 1.5f),
    glm::vec3(-2.0f, 8.0f, 1.5f),
    glm::vec3(2.0f, 8.0f, 1.5f),
    glm::vec3(-2.5f, 2.0f, 1.5f),
    glm::vec3(2.5f, 2.0f, 1.5f),
};
//光照颜色(光源颜色)
glm::vec3 lightColor(1.0f,1.0f,1.0f);

//显示程序
void tutorial(){
    GLFWwindow *window = initWindow("ModelLoading", 800, 600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    Camera* pCamera = CameraController::getCamera();
    glfwSwapInterval(0);
    //着色器初始化
    Shader modelShader("Resources/Shaders/ModelLoading/nanosuit.vs","Resources/Shaders/ModelLoading/nanosuit.frag");
    //模型初始化
    Model model((GLchar*)"Resources/Meshes/nanosuit/nanosuit.obj");
    //绑定控制
    CameraController::bindControl(window);
    pCamera->moveto(glm::vec3(0, 20, 20));
    pCamera->rotate(-40, 0);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    //绘制
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CameraController::update();
        //
        ca.draw();
        model.draw(&modelShader, pCamera);
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void exercise1(){
    GLFWwindow *window=initWindow("ModelLoading-Ex1",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);
    Camera* pCamera = CameraController::getCamera();
    //初始化点光源着色器
    Shader lampShader("Resources/Shaders/ModelLoading/lamp.vs","Resources/Shaders/ModelLoading/lamp.frag");
    lampShader.use();
    glUniform3f(glGetUniformLocation(lampShader.programID,"lampColor"),lightColor.x,lightColor.y,lightColor.z);
    //初始化点光源模型
    std::vector<Shape> lamps;
    for(int i = 0; i < NR_POINT_LIGHTS; ++i) {
        lamps.push_back(Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES));
        lamps[i].scaleTo(0.05f);
        lamps[i].moveTo(lightPos[i]);
    }
    //着色器初始化
    Shader modelShader("Resources/Shaders/ModelLoading/nanosuit2.vs","Resources/Shaders/ModelLoading/nanosuit2.frag");
    //设置灯光参数
    modelShader.use();
    glm::vec3 lightDiffuseColor = lightColor * glm::vec3(0.8f);
    glm::vec3 lightAmbientColor = lightColor * glm::vec3(0.4f);
    glm::vec3 lightSpecularColor = lightColor * glm::vec3(1.0f);
    for(int i=0;i<NR_POINT_LIGHTS;++i){
        char buffer[256];
        sprintf(buffer,"vLight[%d].position",i);
        glUniform3f(glGetUniformLocation(modelShader.programID,buffer),lightPos[i].x,lightPos[i].y,lightPos[i].z);
        sprintf(buffer,"vLight[%d].ambient",i);
        glUniform3f(glGetUniformLocation(modelShader.programID,buffer),lightAmbientColor.x,lightAmbientColor.y,lightAmbientColor.z);
        sprintf(buffer,"vLight[%d].diffuse",i);
        glUniform3f(glGetUniformLocation(modelShader.programID,buffer),lightDiffuseColor.x,lightDiffuseColor.y,lightDiffuseColor.z);
        sprintf(buffer,"vLight[%d].specular",i);
        glUniform3f(glGetUniformLocation(modelShader.programID,buffer),lightSpecularColor.x,lightSpecularColor.y,lightSpecularColor.z);
        sprintf(buffer,"vLight[%d].constant",i);
        glUniform1f(glGetUniformLocation(modelShader.programID,buffer),1.0);
        sprintf(buffer,"vLight[%d].linear",i);
        glUniform1f(glGetUniformLocation(modelShader.programID,buffer),0.09);
        sprintf(buffer,"vLight[%d].quadratic",i);
        glUniform1f(glGetUniformLocation(modelShader.programID,buffer),0.32);
    }
    glUniform1f(glGetUniformLocation(modelShader.programID,"shininess"),32.0f);
    //模型初始化
    Model model((GLchar*)"Resources/Meshes/nanosuit/nanosuit.obj");
    //绑定控制
    CameraController::bindControl(window);
    pCamera->moveto(glm::vec3(0, 20, 20));
    pCamera->rotate(-40, 0);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    ca.showGrid(false);
    //绘制
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        CameraController::update();

        for(int i = 0; i < NR_POINT_LIGHTS; ++i){
            lamps[i].draw(&lampShader, pCamera);
        }
        ca.draw();
        model.draw(&modelShader, pCamera);

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
