/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MODEL_LOADING_HPP
#define MODEL_LOADING_HPP
namespace ModelLoading{
//顶点信息前置声明
extern GLfloat cubeVertices[8*8];
extern GLuint cubeIndices[6];
extern GLuint lampIndices[36];
//光照颜色(光源颜色)
glm::vec3 lightColor(1.0f,1.0f,1.0f);
//物体位置信息
glm::vec3 cubePos(0.0f,-0.5f,0.0f);
glm::vec3 lampPos(1.0f, 1.0f, 1.0f);

//Model类的单元测试
void modelTest(){
    //Init
    GLFWwindow *window=initWindow("ModelLoadingTest",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    //着色器初始化
    Shader cubeShader("shaders/ModelLoading/cube.vs","shaders/ModelLoading/cube.frag");
    cubeShader.use();
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = lightColor * glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = lightColor * glm::vec3(1.0f); // Low influence
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.position"), lampPos[0],lampPos[1],lampPos[2]);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.specular"), specularColor.x,specularColor.y,specularColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "shininess"), 32.0f);
    //用方块形式显示灯
        //初始化灯的着色器
    Shader lampShader("shaders/ModelLoading/lamp.vs","shaders/ModelLoading/lamp.frag");
    lampShader.use();
    glUniform3f(glGetUniformLocation(lampShader.programID,"lampColor"),lightColor.x,lightColor.y,lightColor.z);
        //初始化灯
    Object lamp(cubeVertices,8,POSITIONS_NORMALS_TEXTURES,GL_TRIANGLES,lampIndices,36);
    lamp.setCamera(&CameraController::camera);
    lamp.setShader(&lampShader);
    lamp.moveTo(lampPos);
    lamp.scaleTo(0.2);
    //生成Mesh
        //顶点
    vector<Vertex> vertices(8);
    for(int i=0;i<8;++i){
        int index=8*i;
        vertices[i].Position=glm::vec3(cubeVertices[index],cubeVertices[index+1],cubeVertices[index+2]);
        vertices[i].Normal=glm::vec3(cubeVertices[index+3],cubeVertices[index+4],cubeVertices[index+5]);
        vertices[i].TexCoords=glm::vec2(cubeVertices[index+6],cubeVertices[index+7]);
    }
        //下标
    vector<GLuint> indices(&cubeIndices[0],&cubeIndices[5]+1);
        //纹理
    vector<Texture> textures;
    Texture box("textures/container2.png",TEXTURE_DIFFUSE,GL_BGRA,GL_RGBA);
    Texture boxSpec("textures/container2_specular.png",TEXTURE_SPECULAR,GL_BGRA,GL_RGBA);
    textures.push_back(box);
    textures.push_back(boxSpec);
        //网格
    Mesh mesh(vertices,indices,textures);

    //绑定控制
    CameraController::bindControl(window);
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
        lamp.draw();

        //手动更新摄像机信息
        cubeShader.use();
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.programID, "view"),1,GL_FALSE,CameraController::camera.getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.programID, "projection"),1,GL_FALSE,CameraController::camera.getProjectionMatrixVal());
        glm::mat4 model=glm::mat4();
        model=glm::translate(model,cubePos);
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.programID, "model"),1,GL_FALSE,glm::value_ptr(model));
        mesh.draw(&cubeShader);

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

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
    //绑定控制
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    //FPS计数器
    FPSCounter fc;
    //绘制
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        CameraController::update();

        ca.draw();

        //手动更新摄像机信息
        modelShader.use();
        glUniformMatrix4fv(glGetUniformLocation(modelShader.programID, "view"),1,GL_FALSE,CameraController::camera.getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(modelShader.programID, "projection"),1,GL_FALSE,CameraController::camera.getProjectionMatrixVal());
        glm::mat4 modelMat=glm::mat4();
        modelMat=glm::translate(modelMat,cubePos);
        glUniformMatrix4fv(glGetUniformLocation(modelShader.programID, "model"),1,GL_FALSE,glm::value_ptr(modelMat));
        model.draw();
        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
//顶点信息
GLfloat cubeVertices[8*8]={
    // Positions           // Normals           // Texture Coords
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,//0 .
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,//1
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,//2
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,//3
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,//4
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,//5
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,//6
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,//7
};
GLuint cubeIndices[6]={
    4,0,1,
    1,5,4
};
//顶点索引
GLuint lampIndices[3*2*6]={
    7,3,1,
    1,5,7,

    6,2,0,
    0,4,6,

    4,5,7,
    7,6,4,

    0,1,3,
    3,2,0,

    7,3,2,
    2,6,7,

    5,1,0,
    0,4,5
};
};
#endif // MODEL_LOADING_HPP
