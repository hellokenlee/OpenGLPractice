/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef FRAMEBUFFERS_HPP
#define FRAMEBUFFERS_HPP
namespace Framebuffers{

void tutorial(){
    //
    GLFWwindow* window=initWindow("Blending",800,600);
    showEnviroment();
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    //
    CoordinateAxes ca(&CameraController::camera);
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        CameraController::update();

        ca.draw();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();

}

};
#endif
