/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef NORMAL_MAPPING_HPP
#define NORMAL_MAPPING_HPP
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
bool lock = false;
bool normalMap = false;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    CameraController::keyCallback(window, key, scancode, action, mode);
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
        //锁定视角方向
        lock = !lock;
        if(lock){
            glfwSetCursorPosCallback(window, nullptr);
        }else{
            glfwSetCursorPosCallback(window, CameraController::mouseCallback);
        }
    }
    if(key == GLFW_KEY_N && action == GLFW_PRESS){
        //开启/关闭 法线贴图
        normalMap = !normalMap;
        cout<<"Normal Mapping : "<<(normalMap ? "ON" : "OFF")<<endl;
    }
}
void tutorial(){
    GLFWwindow *window = initWindow("NormalMapping", 800, 600, keyCallback, CameraController::mouseCallback);
    showEnviroment();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/NormalMapping/brick_wall.vs", "shaders/NormalMapping/brick_wall.frag");
    shader.use();
    glUniform3f(glGetUniformLocation(shader.programID, "vLight.position"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1i(glGetUniformLocation(shader.programID, "texture_diffuse1"), 0);
    glUniform1i(glGetUniformLocation(shader.programID, "texture_normal1"), 1);

    CoordinateAxes ca(&CameraController::camera);


    Object wall(planeVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    wall.setCamera(&CameraController::camera);
    wall.setShader(&shader);
    wall.scaleTo(5.0f);

    TextureManager* tm = TextureManager::getManager();
    tm->loadTexture("textures/brickwall.jpg", 0, GL_BGR, GL_SRGB);
    tm->loadTexture("textures/brickwall_normal.jpg", 1, GL_BGR, GL_RGB);

    Camera *cam = &CameraController::camera;

    cout<<"Normal Mapping : "<<(normalMap ? "ON" : "OFF")<<endl;
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        tm->bindTexture(0);
        glActiveTexture(GL_TEXTURE1);
        tm->bindTexture(1);
        shader.use();
        glUniform1i(glGetUniformLocation(shader.programID, "normalMap"), normalMap);
        glUniform3f(glGetUniformLocation(shader.programID, "viewPosition"), cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
        wall.draw();

        glfwSwapBuffers(window);

    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif
