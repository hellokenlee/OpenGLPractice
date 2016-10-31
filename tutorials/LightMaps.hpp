/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef LIGHT_MAPS_HPP
#define LIGHT_MAPS_HPP
namespace LightMaps{
//顶点信息前置声明
extern GLfloat cubeVertices[36*8];

//光照颜色(光源颜色)
glm::vec3 lightColor(1.0f,1.0f,1.0f);

//物体位置信息
glm::vec3 cubePos(0.0f,0.5f,0.0f);
glm::vec3 lampPos(1.2f, 1.5f, 2.0f);

//教程：通过材质和光照影响物体表面颜色
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("LightMaps",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //载入纹理
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container2.png",0,GL_RGBA,GL_BGRA))
        return ;
    //着色器程序初始化
    Shader cubeShader("shaders/LightMaps/cube.vs","shaders/LightMaps/cube.frag");
    Shader lampShader("shaders/LightMaps/lamp.vs","shaders/LightMaps/lamp.frag");
    cubeShader.use();
    GLint matDiffuseLoc = glGetUniformLocation(cubeShader.programID, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(cubeShader.programID, "material.specular");
    GLint matShineLoc = glGetUniformLocation(cubeShader.programID, "material.shininess");
    glUniform1i(matDiffuseLoc, 0);
    glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
    glUniform1f(matShineLoc, 32.0f);

    GLint lightPositionLoc = glGetUniformLocation(cubeShader.programID, "vLight.position");
    GLint lightAmbientLoc = glGetUniformLocation(cubeShader.programID, "vLight.ambient");
    GLint lightDiffuseLoc = glGetUniformLocation(cubeShader.programID, "vLight.diffuse");
    GLint lightSpecularLoc = glGetUniformLocation(cubeShader.programID, "vLight.specular");
    glUniform3f(lightPositionLoc, lampPos[0],lampPos[1],lampPos[2]);
    glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
    glUniform1i(matDiffuseLoc, 0);

    lampShader.use();
    GLint lampColorLoc=glGetUniformLocation(lampShader.programID,"lampColor");
    //物体对象初始化
    Object cube(cubeVertices,36,POSITIONS_NORMALS_TEXTURES,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&cubeShader);
    cube.moveTo(cubePos);

    Object lamp(cubeVertices,36,POSITIONS_NORMALS_TEXTURES,GL_TRIANGLES);
    lamp.setCamera(&CameraController::camera);
    lamp.setShader(&lampShader);
    lamp.moveTo(lampPos);
    lamp.scaleTo(0.4);
    //绑定控制
    glfwSetKeyCallback(window,CameraController::keyCallback);
    glfwSetCursorPosCallback(window,CameraController::mouseCallback);
    glfwSetScrollCallback(window,CameraController::scrollCallback);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //帧数计数器
    FPSCounter fc;
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lamp.shader->use();
        glUniform3f(lampColorLoc,lightColor.x,lightColor.y,lightColor.z);
        lamp.draw();

        cube.shader->use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // Decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.1f); // Low influence
        glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(lightSpecularLoc, lightColor.x, lightColor.y, lightColor.z);
        cube.draw();

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

}

#endif  //  Light_MAPS_HPP
