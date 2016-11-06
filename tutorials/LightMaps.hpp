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

//教程：使用光照贴图（包括漫反射贴图和镜面贴图）
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("LightMaps",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //着色器程序初始化
    Shader cubeShader("shaders/LightMaps/cube.vs","shaders/LightMaps/cube.frag");
    Shader lampShader("shaders/LightMaps/lamp.vs","shaders/LightMaps/lamp.frag");
    cubeShader.use();
    GLint matDiffuseLoc = glGetUniformLocation(cubeShader.programID, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(cubeShader.programID, "material.specular");
    GLint matShineLoc = glGetUniformLocation(cubeShader.programID, "material.shininess");
    glUniform1f(matShineLoc, 32.0f);
    glUniform1i(matDiffuseLoc, 0);
    glUniform1i(matSpecularLoc,1);
    GLint lightPositionLoc = glGetUniformLocation(cubeShader.programID, "vLight.position");
    GLint lightAmbientLoc = glGetUniformLocation(cubeShader.programID, "vLight.ambient");
    GLint lightDiffuseLoc = glGetUniformLocation(cubeShader.programID, "vLight.diffuse");
    GLint lightSpecularLoc = glGetUniformLocation(cubeShader.programID, "vLight.specular");
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = lightColor * glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = lightColor * glm::vec3(1.0f); // Low influence
    glUniform3f(lightPositionLoc, lampPos[0],lampPos[1],lampPos[2]);
    glUniform3f(lightSpecularLoc, specularColor.x,specularColor.y,specularColor.z);
    glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);

    lampShader.use();
    GLint lampColorLoc=glGetUniformLocation(lampShader.programID,"lampColor");
    glUniform3f(lampColorLoc,lightColor.x,lightColor.y,lightColor.z);
    //物体对象初始化
    Object cube(cubeVertices,36,POSITIONS_NORMALS_TEXTURES,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&cubeShader);
    cube.moveTo(cubePos);

    Object* lamp=cube.clone();
    lamp->setShader(&lampShader);
    lamp->moveTo(lampPos);
    lamp->scaleTo(0.4);
    //绑定控制
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //ca.showGrid(false);
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
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lamp->draw();

        cube.shader->use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        cube.draw();

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

//Ex1为调整-观察练习，在这不重写一次
//Ex2：在纹理着色器中翻转specular的值，使得木头反光而铁边不反光
void exercise2(){
    //创建窗口
    GLFWwindow *window=initWindow("LightMapsEx2",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //着色器程序初始化
    Shader cubeShader("shaders/LightMaps/cube.vs","shaders/LightMaps/cubeEx2.frag");
    Shader lampShader("shaders/LightMaps/lamp.vs","shaders/LightMaps/lamp.frag");
    cubeShader.use();
    GLint matDiffuseLoc = glGetUniformLocation(cubeShader.programID, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(cubeShader.programID, "material.specular");
    GLint matShineLoc = glGetUniformLocation(cubeShader.programID, "material.shininess");
    glUniform1f(matShineLoc, 32.0f);
    glUniform1i(matDiffuseLoc, 0);
    glUniform1i(matSpecularLoc,1);
    GLint lightPositionLoc = glGetUniformLocation(cubeShader.programID, "vLight.position");
    GLint lightAmbientLoc = glGetUniformLocation(cubeShader.programID, "vLight.ambient");
    GLint lightDiffuseLoc = glGetUniformLocation(cubeShader.programID, "vLight.diffuse");
    GLint lightSpecularLoc = glGetUniformLocation(cubeShader.programID, "vLight.specular");
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = lightColor * glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = lightColor * glm::vec3(1.0f); // Low influence
    glUniform3f(lightPositionLoc, lampPos[0],lampPos[1],lampPos[2]);
    glUniform3f(lightSpecularLoc, specularColor.x,specularColor.y,specularColor.z);
    glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
    glUniform3f(lightSpecularLoc, lightColor.x, lightColor.y, lightColor.z);

    lampShader.use();
    GLint lampColorLoc=glGetUniformLocation(lampShader.programID,"lampColor");
    glUniform3f(lampColorLoc,lightColor.x,lightColor.y,lightColor.z);
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
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //ca.showGrid(false);
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
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lamp.draw();

        cube.shader->use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        cube.draw();

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

//Ex3：使用带颜色的镜面光照贴图
void exercise3(){
    //创建窗口
    GLFWwindow *window=initWindow("LightMapsEx3",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //着色器程序初始化
    Shader cubeShader("shaders/LightMaps/cube.vs","shaders/LightMaps/cube.frag");
    Shader lampShader("shaders/LightMaps/lamp.vs","shaders/LightMaps/lamp.frag");
    cubeShader.use();
    GLint matDiffuseLoc = glGetUniformLocation(cubeShader.programID, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(cubeShader.programID, "material.specular");
    GLint matShineLoc = glGetUniformLocation(cubeShader.programID, "material.shininess");
    glUniform1f(matShineLoc, 32.0f);
    glUniform1i(matDiffuseLoc, 0);
    glUniform1i(matSpecularLoc,1);
    GLint lightPositionLoc = glGetUniformLocation(cubeShader.programID, "vLight.position");
    GLint lightAmbientLoc = glGetUniformLocation(cubeShader.programID, "vLight.ambient");
    GLint lightDiffuseLoc = glGetUniformLocation(cubeShader.programID, "vLight.diffuse");
    GLint lightSpecularLoc = glGetUniformLocation(cubeShader.programID, "vLight.specular");
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = lightColor * glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = lightColor * glm::vec3(1.0f); // Low influence
    glUniform3f(lightPositionLoc, lampPos[0],lampPos[1],lampPos[2]);
    glUniform3f(lightSpecularLoc, specularColor.x,specularColor.y,specularColor.z);
    glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
    glUniform3f(lightSpecularLoc, lightColor.x, lightColor.y, lightColor.z);

    lampShader.use();
    GLint lampColorLoc=glGetUniformLocation(lampShader.programID,"lampColor");
    glUniform3f(lampColorLoc,lightColor.x,lightColor.y,lightColor.z);
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
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //ca.showGrid(false);
    //帧数计数器
    FPSCounter fc;
    //载入纹理
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container2.png",0,GL_BGRA,GL_RGBA))
        return ;
    if(!tm->loadTexture("textures/container2_specular_color.png",1,GL_BGRA,GL_RGBA))
        return ;
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lamp.draw();

        cube.shader->use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        cube.draw();

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
//光照颜色(光源颜色)
glm::vec3 lightColorEx4(0.0f,0.8f,0.0f);
//Ex4：使用放射性光贴图(Emission Map)模拟光源
void exercise4(){
    //创建窗口
    GLFWwindow *window=initWindow("LightMapsEx4",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //着色器程序初始化
    Shader cubeShader("shaders/LightMaps/cube.vs","shaders/LightMaps/cube.frag");
    Shader lampShader("shaders/LightMaps/lampEx4.vs","shaders/LightMaps/lampEx4.frag");
    cubeShader.use();
    GLint matDiffuseLoc = glGetUniformLocation(cubeShader.programID, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(cubeShader.programID, "material.specular");
    GLint matShineLoc = glGetUniformLocation(cubeShader.programID, "material.shininess");
    glUniform1f(matShineLoc, 32.0f);
    glUniform1i(matDiffuseLoc, 0);
    glUniform1i(matSpecularLoc,1);
    GLint lightPositionLoc = glGetUniformLocation(cubeShader.programID, "vLight.position");
    GLint lightAmbientLoc = glGetUniformLocation(cubeShader.programID, "vLight.ambient");
    GLint lightDiffuseLoc = glGetUniformLocation(cubeShader.programID, "vLight.diffuse");
    GLint lightSpecularLoc = glGetUniformLocation(cubeShader.programID, "vLight.specular");
    glm::vec3 diffuseColor = lightColorEx4 * glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = lightColorEx4 * glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = lightColorEx4 * glm::vec3(1.0f); // Low influence
    glUniform3f(lightPositionLoc, lampPos[0],lampPos[1],lampPos[2]);
    glUniform3f(lightSpecularLoc, specularColor.x,specularColor.y,specularColor.z);
    glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
    glUniform3f(lightSpecularLoc, lightColorEx4.x, lightColorEx4.y, lightColorEx4.z);

    lampShader.use();
    GLint lampColorLoc=glGetUniformLocation(lampShader.programID,"lampColor");
    GLint lampEmissionLoc=glGetUniformLocation(lampShader.programID,"fEmission");
    glUniform3f(lampColorLoc,lightColorEx4.x,lightColorEx4.y,lightColorEx4.z);
    glUniform1i(lampEmissionLoc,0);
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
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //ca.showGrid(false);
    //帧数计数器
    FPSCounter fc;
    //载入纹理
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container2.png",0,GL_BGRA,GL_RGBA))
        return ;
    if(!tm->loadTexture("textures/container2_specular.png",1,GL_BGRA,GL_RGBA))
        return ;
    if(!tm->loadTexture("textures/matrix.jpg",2))
        return ;
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lamp.shader->use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(2);
        lamp.draw();

        cube.shader->use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
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
