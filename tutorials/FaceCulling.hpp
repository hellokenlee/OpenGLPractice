/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef FACE_CULLING_HPP
#define FACE_CULLING_HPP
namespace FaceCulling{

extern GLfloat cubeVertices[36*5];

void tutorial(){
    // Init window
    GLFWwindow* window=initWindow("Blending",800,600);
    showEnviroment();
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Cube obejct define
    Object cube(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    Shader shader("shaders/FaceCulling/scene1.vs","shaders/FaceCulling/scene1.frag");
    cube.setShader(&shader);
    cube.setCamera(&CameraController::camera);
    // Texutures
    TextureManager *tm=TextureManager::getManager();
    tm->loadTexture("textures/container2.png",0,GL_BGRA,GL_RGBA);
    //
    CoordinateAxes ca(&CameraController::camera);
    //
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        tm->bindTexture(0);
        cube.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void exercise1(){
    // Init window
    GLFWwindow* window=initWindow("Blending",800,600);
    showEnviroment();
    // Set the required callback functions
    CameraController::bindControl(window);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Cube obejct define
    Object cube(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    Shader shader("shaders/FaceCulling/scene1.vs","shaders/FaceCulling/scene1.frag");
    cube.setShader(&shader);
    cube.setCamera(&CameraController::camera);
    // Texutures
    TextureManager *tm=TextureManager::getManager();
    tm->loadTexture("textures/container2.png",0,GL_BGRA,GL_RGBA);
    //
    CoordinateAxes ca(&CameraController::camera);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        tm->bindTexture(0);
        cube.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

/*
    Remember: to specify vertices in a counter-clockwise winding order you need to visualize the triangle
    as if you're in front of the triangle and from that point of view, is where you set their order.

    To define the order of a triangle on the right side of the cube for example, you'd imagine yourself looking
    straight at the right side of the cube, and then visualize the triangle and make sure their order is specified
    in a counter-clockwise order. This takes some practice, but try visualizing this yourself and see that this
    is correct.
*/
GLfloat cubeVertices[] = {
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
};
#endif // FACE_CULLING_HPP
