/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MATERIALS_HPP
#define MATERIALS_HPP
namespace Materials{
//顶点信息前置声明
extern GLfloat cubeVertices[36*6];

//
glm::vec3 lightColor;

//物体位置信息
glm::vec3 cubePos(0.0f,0.5f,0.0f);
glm::vec3 lampPos(1.2f, 1.5f, 2.0f);

//教程：通过材质和光照影响物体表面颜色
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("Materials",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //着色器程序初始化
    Shader cubeShader("shaders/Materials/cube.vs","shaders/Materials/cube.frag");
    Shader lampShader("shaders/Materials/lamp.vs","shaders/Materials/lamp.frag");
    cubeShader.use();
    GLint matAmbientLoc = glGetUniformLocation(cubeShader.programID, "material.ambient");
    GLint matDiffuseLoc = glGetUniformLocation(cubeShader.programID, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(cubeShader.programID, "material.specular");
    GLint matShineLoc = glGetUniformLocation(cubeShader.programID, "material.shininess");
    glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
    glUniform1f(matShineLoc, 32.0f);

    GLint lightPositionLoc = glGetUniformLocation(cubeShader.programID, "vLight.position");
    GLint lightAmbientLoc = glGetUniformLocation(cubeShader.programID, "vLight.ambient");
    GLint lightDiffuseLoc = glGetUniformLocation(cubeShader.programID, "vLight.diffuse");
    GLint lightSpecularLoc = glGetUniformLocation(cubeShader.programID, "vLight.specular");
    glUniform3f(lightPositionLoc, lampPos[0],lampPos[1],lampPos[2]);
    glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

    lampShader.use();
    GLint lampColorLoc=glGetUniformLocation(lampShader.programID,"lampColor");
    //物体对象初始化
    Object cube(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&cubeShader);
    cube.moveTo(cubePos);

    Object lamp(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
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
        lightColor.x = sin(glfwGetTime() * 2.0f * 0.3);
        lightColor.y = sin(glfwGetTime() * 0.7f * 0.3);
        lightColor.z = sin(glfwGetTime() * 1.3f * 0.3);
        glUniform3f(lampColorLoc,lightColor.x,lightColor.y,lightColor.z);
        lamp.draw();

        cube.shader->use();
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


//练习1： 使用下面的材质包数据选一个喜欢的做出来，我做了一个绿松石～
//http://devernay.free.fr/cours/opengl/materials.html
void exercise1(){
    //创建窗口
    GLFWwindow *window=initWindow("Materials",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //着色器程序初始化
    Shader cubeShader("shaders/Materials/cube.vs","shaders/Materials/cube.frag");
    Shader lampShader("shaders/Materials/lamp.vs","shaders/Materials/lamp.frag");
    cubeShader.use();
    GLint matAmbientLoc = glGetUniformLocation(cubeShader.programID, "material.ambient");
    GLint matDiffuseLoc = glGetUniformLocation(cubeShader.programID, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(cubeShader.programID, "material.specular");
    GLint matShineLoc = glGetUniformLocation(cubeShader.programID, "material.shininess");
    glUniform3f(matAmbientLoc, 0.1,0.18725,0.1745);
    glUniform3f(matDiffuseLoc, 0.396,0.74151,0.69102);
    glUniform3f(matSpecularLoc, 0.297254,0.30829,0.306678);
    glUniform1f(matShineLoc, 0.1*128);

    GLint lightPositionLoc = glGetUniformLocation(cubeShader.programID, "vLight.position");
    GLint lightAmbientLoc = glGetUniformLocation(cubeShader.programID, "vLight.ambient");
    GLint lightDiffuseLoc = glGetUniformLocation(cubeShader.programID, "vLight.diffuse");
    GLint lightSpecularLoc = glGetUniformLocation(cubeShader.programID, "vLight.specular");
    glUniform3f(lightPositionLoc, lampPos[0],lampPos[1],lampPos[2]);
    glUniform3f(lightAmbientLoc, 1.0f, 1.0f, 1.0f);
    glUniform3f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f);
    glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

    lampShader.use();
    GLint lampColorLoc=glGetUniformLocation(lampShader.programID,"lampColor");
    glUniform3f(lampColorLoc,1.0f, 1.0f, 1.0f);
    //物体对象初始化
    Object cube(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&cubeShader);
    cube.moveTo(cubePos);

    Object lamp(cubeVertices,36,POSITIONS_NORMALS,GL_TRIANGLES);
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
        lamp.draw();
        cube.draw();

        glfwSwapBuffers(window);
        fc.update();
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
#endif  //  MATERIALS_HPP
