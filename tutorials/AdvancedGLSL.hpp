/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef ADVANCED_GLSL_HPP
#define ADVANCED_GLSL_HPP
namespace AdvancedGLSL{
extern GLfloat cubeVertices[6*6*5];
extern GLfloat cubePositions[6*6*3];
extern GLfloat cubeNormals[6*6*3];
extern GLfloat cubeTexCoords[6*6*2];


// 使用GLSL内置变量对Shader显示效果控制
void tutorial(){
    // init
    GLFWwindow* window=initWindow("AdvancedGLSL", 800, 600);
    showEnviroment();
    // setup control
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);
    // gsls options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    //
    CoordinateAxes ca(&CameraController::camera);
    //
    Shader cubeShader("shaders/AdvancedGLSL/scene.vs", "shaders/AdvancedGLSL/two_textures.frag");
    //
    Object cube(cubeVertices, 36, POSITIONS_TEXTURES, GL_TRIANGLES);
    cube.setShader(&cubeShader);
    cube.setCamera(&CameraController::camera);
    cube.scaleTo(10.0f);
    //
    TextureManager *tm = TextureManager::getManager();
    //!TODO： 增加导入纹理错误检查
    if(!tm->loadTexture("textures/container2.png", 0, GL_BGRA, GL_RGBA))
        return ;
    if(!tm->loadTexture("textures/cloth.jpg", 1, GL_BGR, GL_RGB))
     return;
    // main loop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        ca.draw();

        cubeShader.use();
        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glUniform1i(glGetUniformLocation(cubeShader.programID, "texture0"), 0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        glUniform1i(glGetUniformLocation(cubeShader.programID, "texture1"), 1);
        cube.draw();

        glfwSwapBuffers(window);
    }
    // free resources
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 使用UBO来完成AdvancedData章节的渲染。使用UBO传递摄像机的View和Projection矩阵
void exercise(){
    GLFWwindow *window=initWindow("AdvancedData",800,600);
    showEnviroment();
    glfwSwapInterval(0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);
    glEnable(GL_DEPTH_TEST);
    CoordinateAxes ca(&CameraController::camera);
    //
    //
    Shader cubeShader("shaders/AdvancedGLSL/scene2.vs","shaders/AdvancedGLSL/scene2.frag");
    // 申请UBO
    GLuint UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 256, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //设置Shader的UBO绑定点。4.2+可以在GLSL中使用“layout(std140, binding = 2) uniform Lights { ... };”
    glUniformBlockBinding(cubeShader.programID, glGetUniformBlockIndex(cubeShader.programID, "CameraMatrix"), 0);
    //把ubo绑定在同一个绑定点身上
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);

    //纹理载入
    TextureManager* tm=TextureManager::getManager();
    if(!tm->loadTexture("textures/container2.png",0,GL_BGRA,GL_RGBA))
        return ;
    //箱子物体 //使用glBufferSubData 分批导入顶点位置，法线和纹理坐标。存储形式为PPP...NNN...TTT。而以往的储存形式为PNTPNTPNT...
    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
            //申请空间
        glBufferData(GL_ARRAY_BUFFER,2000*sizeof(GLfloat),nullptr,GL_STATIC_DRAW);
            //位置
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubePositions), &cubePositions);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
        glEnableVertexAttribArray(0);
            //法线
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubePositions), sizeof(cubeNormals), &cubeNormals);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)(sizeof(cubePositions)));
        glEnableVertexAttribArray(1);
            //纹理坐标
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubePositions) + sizeof(cubeNormals), sizeof(cubeTexCoords), &cubeTexCoords);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),(GLvoid*)(sizeof(cubePositions)+sizeof(cubeNormals)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        CameraController::update();

        ca.draw();
        //更新UBO数据
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(CameraController::camera.view));
        glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(CameraController::camera.projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        //绘制物体
        tm->bindTexture(0);
        cubeShader.use();
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.programID,"model"),1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }
}

//
GLfloat cubePositions[] = {
    // Positions
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

GLfloat cubeNormals[] ={
     // Normals
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,

     0.0f,  0.0f, 1.0f,
     0.0f,  0.0f, 1.0f,
     0.0f,  0.0f, 1.0f,
     0.0f,  0.0f, 1.0f,
     0.0f,  0.0f, 1.0f,
     0.0f,  0.0f, 1.0f,

    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,

     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,

     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
};
GLfloat cubeTexCoords[]={
    // Texture Coords
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,

    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
};
GLfloat cubeVertices[6*6*5] = {
    // Positions          // Texture Coords
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
};
}
#endif // ADVANCED_GLSL_HPP
