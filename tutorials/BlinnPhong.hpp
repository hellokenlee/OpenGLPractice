/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef BLINN_PHONG_HPP
#define BLINN_PHONG_HPP
namespace BlinnPhong{
#include <cstdio>
//
GLfloat planeVertices[] = {
    // Positions          // Normals         // Texture Coords
     8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 0.0f,
    -8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
    -8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 5.0f,

     8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 0.0f,
    -8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 5.0f,
     8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 5.0f
};
//
glm::vec3 lightPos = glm::vec3(0.0, 0.0, 0.0);
bool blinn = false;
bool lock = false;
//
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    CameraController::keyCallback(window, key, scancode, action, mode);
    if(key == GLFW_KEY_B && action == GLFW_PRESS){
        blinn = !blinn;
        cout<<"Switched to "<<(blinn?"Blin Phong":"Pure Phong")<<" Mode"<<endl;
    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
        lock = !lock;
        if(lock){
            glfwSetCursorPosCallback(window, nullptr);
        }else{
            glfwSetCursorPosCallback(window, CameraController::mouseCallback);
        }
    }
}
void tutorial(){
    GLFWwindow *window = initWindow("BlinnPhong", 800, 600, keyCallback, CameraController::mouseCallback);
    showEnviroment();

    CameraController::camera.moveto(glm::vec3(0.0f, 1.0f, 3.0f));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/BlinnPhong/plane.vs", "shaders/BlinnPhong/plane.frag");
    shader.use();
    glUniform3f(glGetUniformLocation(shader.programID, "vLight.position"), lightPos.x, lightPos.y, lightPos.z);

    CoordinateAxes ca(&CameraController::camera);


    Object plane(planeVertices, 6, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    plane.setCamera(&CameraController::camera);
    plane.setShader(&shader);

    TextureManager* tm = TextureManager::getManager();
    tm->loadTexture("textures/wood.jpg", 0, GL_BGR, GL_RGB);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tm->bindTexture(0);
        shader.use();
        glUniform1i(glGetUniformLocation(shader.programID, "blinn"), (int)blinn);
        plane.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif
