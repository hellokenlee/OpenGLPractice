/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MULTIPLE_LIGHTS_HPP
#define MULTIPLE_LIGHTS_HPP
namespace MultipleLights{
//顶点信息前置声明
extern GLfloat cubeVertices[36*8];
//位置信息前置声明
extern glm::vec3 cubePositions[10];
//点光源位置前置声明
extern glm::vec3 pointLightPositions[4];
// 实现三种光源混合: 平行光，点光源，和手电筒
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("MultipliesLights",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);
    //着色器程序初始化1
    Shader cubeShader("shaders/MultipleLights/cube.vs","shaders/MultipleLights/cube.frag");
    Shader lampShader("shaders/MultipleLights/lamp.vs","shaders/MultipleLights/lamp.frag");
    //设置Uniforms
    cubeShader.use();
    //设置材质Uniform
    glUniform1f(glGetUniformLocation(cubeShader.programID, "fMaterial.shininess"), 32.0f);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "fMaterial.diffuse"), 0);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "fMaterial.specular"),1);
    //设置定向光uniform
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vDirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vDirLight.specular"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vDirLight.ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vDirLight.diffuse"), 0.4f, 0.4f, 0.4f);
    //设置聚光灯uniforms
    glUniform1f(glGetUniformLocation(cubeShader.programID, "fSpotLight.innerCutOff"), glm::cos(glm::radians(12.5f)));
    glUniform1f(glGetUniformLocation(cubeShader.programID, "fSpotLight.outerCutOff"), glm::cos(glm::radians(17.0f)));
    glUniform3f(glGetUniformLocation(cubeShader.programID, "fSpotLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "fSpotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "fSpotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "fSpotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "fSpotLight.linear"), 0.09);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "fSpotLight.quadratic"), 0.032);
    //设置4个点光源
    for(int i=0;i<4;++i){
        char buff[1024];
        sprintf(buff,"vPointLights[%d].position",i);
        glUniform3f(glGetUniformLocation(cubeShader.programID, buff), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
        sprintf(buff,"vPointLights[%d].specular",i);
        glUniform3f(glGetUniformLocation(cubeShader.programID, buff), 1.0f, 1.0f, 1.0f);
        sprintf(buff,"vPointLights[%d].ambient",i);
        glUniform3f(glGetUniformLocation(cubeShader.programID, buff), 0.05f, 0.05f, 0.05f);
        sprintf(buff,"vPointLights[%d].diffuse",i);
        glUniform3f(glGetUniformLocation(cubeShader.programID, buff), 0.8f, 0.8f, 0.8f);
        sprintf(buff,"vPointLights[%d].constant",i);
        glUniform1f(glGetUniformLocation(cubeShader.programID, buff), 1.0f);
        sprintf(buff,"vPointLights[%d].linear",i);
        glUniform1f(glGetUniformLocation(cubeShader.programID, buff), 0.09);
        sprintf(buff,"vPointLights[%d].quadratic",i);
        glUniform1f(glGetUniformLocation(cubeShader.programID, buff), 0.032);
    }
    //设置Uniforms
    lampShader.use();
    glUniform3f(glGetUniformLocation(lampShader.programID,"lampColor"), 1.0f, 1.0f, 1.0f);
    //物体对象初始化
    Object cubeMother(cubeVertices,36,POSITIONS_NORMALS_TEXTURES,GL_TRIANGLES);
    cubeMother.setCamera(&CameraController::camera);
    cubeMother.setShader(&cubeShader);
    Object* cubes[10];
    for(int i=0;i<10;++i){
        cubes[i]=cubeMother.clone();
        cubes[i]->model=glm::translate(glm::mat4(),cubePositions[i]);
        GLfloat angle=20.0f*i;
        cubes[i]->model=glm::rotate(cubes[i]->model,angle,glm::vec3(1.0f, 0.3f, 0.5f));
    }
    //点光源们初始化
    cubeMother.setShader(&lampShader);
    Object* lamps[4];
    for(int i=0;i<4;++i){
        lamps[i]=cubeMother.clone();
        lamps[i]->moveTo(pointLightPositions[i]);
        lamps[i]->scaleTo(0.2);
    }
    //绑定控制
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    ca.showGrid(false);
    //帧数计数器
    FPSCounter fc;
    //载入纹理
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container2.png",0,GL_BGRA,GL_RGBA))
        return ;
    if(!tm->loadTexture("textures/container2_specular.png",1,GL_BGRA,GL_RGBA))
        return ;
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //绘制坐标轴
        ca.draw();
        //绘制点光源方块
        for(int i=0;i<4;++i)
            lamps[i]->draw();
        //绘制物体
        cubeMother.shader->use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        for(int i=0;i<10;++i)
            cubes[i]->draw();
        //置换缓冲
        glfwSwapBuffers(window);
        fc.update();
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
glm::vec3 cubePositions[10] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

}       //  namespace MultipleLights
#endif  //  MULTIPLE_LIGHTS_HPP
