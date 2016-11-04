/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef LIGHT_CASTERS_HPP
#define LIGHT_CASTERS_HPP
namespace LightCasters{
//顶点信息前置声明
extern GLfloat cubeVertices[36*8];
//位置信息前置声明
extern glm::vec3 cubePositions[10];
//光照颜色(光源颜色)
glm::vec3 lightColor(1.0f,1.0f,1.0f);
//光照的方向
glm::vec3 lightDir(-0.2f,-1.0f,-0.3f);
//教程实现
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("LightCasters",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //着色器程序初始化
    Shader cubeShader("shaders/LightMaps/cube.vs","shaders/LightMaps/cube.frag");
    cubeShader.use();
    glUniform1f(glGetUniformLocation(cubeShader.programID, "material.shininess"), 32.0f);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "material.specular"),1);

    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = lightColor * glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = lightColor * glm::vec3(1.0f); // Low influence
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.direction"), lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.specular"), specularColor.x, specularColor.y, specularColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);

    //物体对象初始化
    Object cubeMother(cubeVertices,36,POSITIONS_NORMALS_TEXTURES,GL_TRIANGLES);
    cubeMother.setCamera(&CameraController::camera);
    cubeMother.setShader(&cubeShader);
    Object* cubes[10];
    for(int i=0;i<10;++i){
        cubes[i]=cubeMother.clone();
        cubes[i]->moveTo(cubePositions[i]);
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
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        cubeMother.shader->use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        for(int i=0;i<10;++i){
            cubes[i]->draw();
        }

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}










//位置信息
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
#endif  //  LIGHT_CASTERS
