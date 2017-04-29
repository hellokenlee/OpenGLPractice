/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef STELLA_HPP
#define STELLA_HPP
namespace Stella{
#include <random>
#include <iostream>
using namespace std;

extern GLfloat squareVertices[6 * 5];

const GLuint SQUARES_NUM = 10000;
const glm::vec3 CENTER = glm::vec3(0.0f, 0.0f, 0.0f);
const GLfloat RADIUS = 150.0f;
const GLuint COLOR_NUM = 4;
const glm::vec3 COLORS[] = {
    glm::vec3(0.407, 0.666, 0.886),
    glm::vec3(0.839, 0.878, 0.549),
    glm::vec3(0.000, 0.090, 0.839),
    glm::vec3(0.890, 0.458, 0.772),
    glm::vec3(0.486, 0.878, 0.529)
};
//
void tutorial(){
    GLFWwindow *window = initWindow("Stella!", 800, 600);
    showEnviroment();
    cout<<"Stella! SQUARES_NUM: "<<SQUARES_NUM<<endl;
    glfwSwapInterval(0);

    CameraController::bindControl(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::camera.setPerspective(45.0f, 4.0f / 3.0f, 0.1f, 5000.0f);
    CameraController::camera.moveto(glm::vec3(0.0f, 0.0f, 1200.0f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CoordinateAxes ca(&CameraController::camera);
    FPSCounter fc;

    Shader shader("shaders/Stella/stella.vs","shaders/Stella/stella.frag");

    //生成随机位置,随机颜色
    default_random_engine generator;
    uniform_int_distribution<int> distributionColors(0, COLOR_NUM);
    uniform_real_distribution<float> distributionAlpha(0.0, 0.4);
    uniform_real_distribution<float> distributionUni(-1.0, 1.0);
    uniform_real_distribution<float> distributionAngle(3.141592653f * 0, 3.141592653f * 0.5);
    glm::mat4 *positions = new glm::mat4[SQUARES_NUM];
    glm::vec4 *colors = new glm::vec4[SQUARES_NUM];
    float x, y, z, r, g, b, a;
    int c;
    glm::mat4 model;
    for(unsigned int i = 0; i < SQUARES_NUM; ++i){
        model = glm::mat4();
        //translation
        x = distributionUni(generator) * distributionUni(generator) * 1000.0f;
        y = distributionUni(generator) * distributionUni(generator) * 1000.0f;
        z = distributionUni(generator) * distributionUni(generator) * 2000.0f;
        model = glm::translate(model, glm::vec3(x, y, z));
        //rotation : 随机绕x 或者绕z 或者绕y 旋转
        GLfloat angle = distributionAngle(generator);
        if(y > 0){
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        } else {
            model = glm::rotate(model, -angle, glm::vec3(1.0f, 0.0f, 0.0f));
        }

        positions[i] = model;
        //color
        c = distributionColors(generator);
        r = COLORS[c].r;
        g = COLORS[c].g;
        b = COLORS[c].b;
        if(z < -500){
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
        }
        a = distributionAlpha(generator) + 0.6f;
        colors[i] = glm::vec4(r, g, b, a);
    }
    // 图形初始化
    GLuint vao, vbo, ia_vbo1, ia_vbo2;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ia_vbo1);
    glGenBuffers(1, &ia_vbo2);
    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);// positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(1);// texture coords
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, ia_vbo1);
            glBufferData(GL_ARRAY_BUFFER, SQUARES_NUM * sizeof(glm::mat4), positions, GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);// model1
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(0 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(3);// model2
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(1 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(4);// model3
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);// model4
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(3 * sizeof(glm::vec4)));
        glBindBuffer(GL_ARRAY_BUFFER, ia_vbo2);
            glBufferData(GL_ARRAY_BUFFER, SQUARES_NUM * sizeof(glm::vec4), colors, GL_STATIC_DRAW);
            glEnableVertexAttribArray(6);// colors
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    glBindVertexArray(0);

    // Main Loop
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        CameraController::update();

        shader.use();
        glUniformMatrix4fv(glGetUniformLocation(shader.programID,"view"),1,GL_FALSE,CameraController::camera.getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(shader.programID,"projection"),1,GL_FALSE,CameraController::camera.getProjectionMatrixVal());
        glBindVertexArray(vao);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, SQUARES_NUM);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 正方形顶点
GLfloat squareVertices[6 * 5]={
    // Positions         // TexCoords
     2.0f, -2.0f,  0.0f,  1.0f, 1.0f,
    -2.0f, -2.0f,  0.0f,  0.0f, 1.0f,
     2.0f,  2.0f,  0.0f,  1.0f, 0.0f,

     2.0f,  2.0f,  0.0f,  1.0f, 0.0f,
    -2.0f,  2.0f,  0.0f,  0.0f, 0.0f,
    -2.0f, -2.0f,  0.0f,  0.0f, 1.0f
};
};
#endif // STELLA_HPP
