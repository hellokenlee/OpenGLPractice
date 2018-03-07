/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef NORMAL_MAPPING_CPP
#define NORMAL_MAPPING_CPP

namespace NormalMapping{

glm::vec3 lightPos = glm::vec3(2.0, 2.0, 1.0);

GLfloat planeVertices[] = {
    // Positions         // TexCoords
     1.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  0.0f,  1.0f, 0.0f,

     1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f, -1.0f,  0.0f,  0.0f, 1.0f
};
GLfloat planeVertices2[] = {
    // Positions         // TexCoords
     1.0f, -1.0f,  0.0f,  1.0f, 1.0f,  //0
     1.0f,  1.0f,  0.0f,  1.0f, 0.0f,  //1
    -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,  //2
    -1.0f, -1.0f,  0.0f,  0.0f, 1.0f   //3
};
GLuint planeIndices[] = {
    0, 3, 1,
    1, 2, 3
};
bool lock = false;
bool normalMap = false;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    CameraController::keyCallback(window, key, scancode, action, mode);
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
        //锁定视角方向
        lock = !lock;
        if(lock){
            glfwSetCursorPosCallback(window, nullptr);
            cout<<"Lock Control!"<<endl;
        }else{
            glfwSetCursorPosCallback(window, CameraController::mouseCallback);
            cout<<"UnLock Control!"<<endl;
        }
    }
    if(key == GLFW_KEY_N && action == GLFW_PRESS){
        //开启/关闭 法线贴图
        normalMap = !normalMap;
        cout<<"Normal Mapping : "<<(normalMap ? "ON" : "OFF")<<endl;
    }
}
void tutorial(){
    //
    GLFWwindow *window = initWindow("NormalMapping", 800, 600);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, CameraController::mouseCallback);
    showEnviroment();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Camera* pCamera = CameraController::getCamera();
    pCamera->moveto(glm::vec3(0.0f, 0.0f, 10.0f));
    glEnable(GL_DEPTH_TEST);
    //
    Shader shader("Resources/Shaders/NormalMapping/brick_wall2.vs", "Resources/Shaders/NormalMapping/brick_wall2.frag");
    //
    Shape wall(planeVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    wall.scaleTo(5.0f);
    wall.setModelMat(glm::rotate(wall.getModelMat(), glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    // 计算TBN矩阵
    glm::vec3 TBN[3 * 2];
    glm::vec3 normal(0.0f, 0.0f, 1.0f);
    for(int i = 0; i < 2; ++i){
        glm::vec3 pos1(planeVertices[i * 15 + 0], planeVertices[i * 15 + 1], planeVertices[i * 15 + 2]);
        glm::vec3 pos2(planeVertices[i * 15 + 5], planeVertices[i * 15 + 6], planeVertices[i * 15 + 7]);
        glm::vec3 pos3(planeVertices[i * 15 + 10], planeVertices[i * 15 + 11], planeVertices[i * 15 + 12]);

        glm::vec2 uv1(planeVertices[i * 15 + 3], planeVertices[i * 15 + 4]);
        glm::vec2 uv2(planeVertices[i * 15 + 8], planeVertices[i * 15 + 9]);
        glm::vec2 uv3(planeVertices[i * 15 + 13], planeVertices[i * 15 + 14]);

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 -uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
        GLfloat f = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV2.x * deltaUV1.y));
        TBN[i * 3 + 0].x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        TBN[i * 3 + 0].y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        TBN[i * 3 + 0].z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        TBN[i * 3 + 0] = glm::normalize(TBN[i * 3 + 0]);

        TBN[i * 3 + 1].x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        TBN[i * 3 + 1].y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        TBN[i * 3 + 1].z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        TBN[i * 3 + 1] = glm::normalize(TBN[i * 3 + 1]);

        TBN[i * 3 + 2] = normal;
    }
    // 绑定TBN
    GLuint TBN_IA_VBO;
    glGenBuffers(1, &TBN_IA_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBN_IA_VBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &TBN[0], GL_STATIC_DRAW);
        glBindVertexArray(wall.VAO);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (GLvoid*)(0 * sizeof(glm::vec3)));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (GLvoid*)(1 * sizeof(glm::vec3)));
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (GLvoid*)(2 * sizeof(glm::vec3)));

            glVertexAttribDivisor(2, 1);
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
        glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //
    Texture tex0("Resources/Textures/brickwall.jpg", GL_BGR, GL_SRGB);
    Texture tex1("Resources/Textures/brickwall_normal.jpg", GL_BGR, GL_RGB);
    //
    cout<<"Normal Mapping : "<<(normalMap ? "ON" : "OFF")<<endl;
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        shader.use();
        glUniform1i(glGetUniformLocation(shader.programID, "normalMap"), normalMap);
        glUniform3f(glGetUniformLocation(shader.programID, "viewPosition"),
                    pCamera->cameraPos.x, pCamera->cameraPos.y, pCamera->cameraPos.z);
        glUniform1i(glGetUniformLocation(shader.programID, "texture_diffuse1"), 0);
        glUniform1i(glGetUniformLocation(shader.programID, "texture_normal1"), 1);
        glUniform3f(glGetUniformLocation(shader.programID, "vLight.position"), lightPos.x, lightPos.y, lightPos.z);
        wall.scaleTo(5.0f);
        wall.setModelMat(glm::rotate(wall.getModelMat(), glm::radians((GLfloat)(glfwGetTime() * -10.0)), glm::vec3(1.0f, 0.0f, 0.0f)));
        tex0.use(0);
        tex1.use(1);
        wall.draw(&shader, pCamera);
        //
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif
