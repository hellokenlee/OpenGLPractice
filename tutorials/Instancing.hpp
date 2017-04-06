/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef INSTANCING_HPP
#define INSTANCING_HPP
namespace Instancing{

#define ROCK_AMOUNT 10000
void tutorial_without_instancing(){
    GLFWwindow *window = initWindow("Instancing", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);

    CameraController::bindControl(window);
    CameraController::camera.moveto(glm::vec3(0.0f, 5.0f, 50.0f));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    CoordinateAxes ca(&CameraController::camera);
    FPSCounter fc;

    Shader plaentShader("shaders/Instancing/model.vs", "shaders/Instancing/model.frag");

    Model planet((GLchar*)"textures/planet/planet.obj");
    planet.setCamera(&CameraController::camera);
    planet.setShader(&plaentShader);

    Model rock((GLchar*)"textures/rock/rock.obj");
    rock.setCamera(&CameraController::camera);
    rock.setShader(&plaentShader);

    //随机生产ROCK_AMOUNT个model矩阵
    glm::mat4 modelMatrices[ROCK_AMOUNT];
    srand(glfwGetTime());
    GLfloat radius = 50.0f;
    GLfloat offset = 5.0f;
    cout<<sizeof(modelMatrices)<<endl;
    for(int i = 0; i < ROCK_AMOUNT; ++i){
        glm::mat4 model;
        // 1. Translation: displace along circle with 'radius' in range [-offset, offset]
        GLfloat angle = (GLfloat)i / (GLfloat)ROCK_AMOUNT * 360.0f;
        GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat x = sin(angle) * radius + displacement;
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat y = displacement * 0.4f; // y value has smaller displacement
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2. Scale: Scale between 0.05 and 0.25f
        GLfloat scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
        // 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
        GLfloat rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        // 4. Now add to list of matrices
        modelMatrices[i] = model;
    }

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        //ca.draw();
        // 绘制行星
        glm::mat4 planetModel;
        planetModel = glm::translate(planetModel, glm::vec3(0.0f, -5.0f, 0.0f));
        planetModel = glm::scale(planetModel, glm::vec3(4.0f, 4.0f, 4.0f));
        planet.modelMat = planetModel;
        planet.draw();
        //绘制陨石
        for(int i = 0; i < ROCK_AMOUNT; ++i){
            rock.modelMat = modelMatrices[i];
            rock.draw();
        }

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void tutorial_with_instancing_by_IA(){
    GLFWwindow *window = initWindow("Instancing", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);

    CameraController::bindControl(window);
    CameraController::camera.moveto(glm::vec3(0.0f, 5.0f, 50.0f));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    CoordinateAxes ca(&CameraController::camera);
    FPSCounter fc;
    Camera *cam = &CameraController::camera;

    Shader plaentShader("shaders/Instancing/model.vs", "shaders/Instancing/model.frag");
    Shader rockShader("shaders/Instancing/rocks_instancing2.vs", "shaders/Instancing/rocks_instancing.frag");

    Model planet((GLchar*)"textures/planet/planet.obj");
    planet.setCamera(cam);
    planet.setShader(&plaentShader);
    glm::mat4 planetModel;
    planetModel = glm::translate(planetModel, glm::vec3(0.0f, -5.0f, 0.0f));
    planetModel = glm::scale(planetModel, glm::vec3(4.0f, 4.0f, 4.0f));
    planet.modelMat = planetModel;

    Model rock((GLchar*)"textures/rock/rock.obj");

    //随机生产ROCK_AMOUNT个model矩阵
    glm::mat4 modelMatrices[ROCK_AMOUNT];
    srand(glfwGetTime());
    GLfloat radius = 50.0f;
    GLfloat offset = 5.0f;
    for(int i = 0; i < ROCK_AMOUNT; ++i){
        glm::mat4 model;
        // 1. Translation: displace along circle with 'radius' in range [-offset, offset]
        GLfloat angle = (GLfloat)i / (GLfloat)ROCK_AMOUNT * 360.0f;
        GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat x = sin(angle) * radius + displacement;
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat y = displacement * 0.4f; // y value has smaller displacement
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2. Scale: Scale between 0.05 and 0.25f
        GLfloat scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
        // 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
        GLfloat rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        // 4. Now add to list of matrices
        modelMatrices[i] = model;
    }

    // 导入model矩阵到VBO
    GLuint IA_VBO;
    glGenBuffers(1, &IA_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, IA_VBO);
    glBufferData(GL_ARRAY_BUFFER, ROCK_AMOUNT * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    // 写入VAP
    glBindVertexArray(rock.meshes[0].VAO);
        // 绑定实例数组
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        // 绘制行星
        planet.draw();

        //绘制陨石
        rockShader.use();
        glUniformMatrix4fv(glGetUniformLocation(rockShader.programID,"view"),1,GL_FALSE,cam->getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(rockShader.programID,"projection"),1,GL_FALSE,cam->getProjectionMatrixVal());
        glBindVertexArray(rock.meshes[0].VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, rock.meshes[0].vertices.size(), ROCK_AMOUNT);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
void tutorial_with_instancing_by_UBO(){
    GLFWwindow *window = initWindow("Instancing", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);

    CameraController::bindControl(window);
    CameraController::camera.moveto(glm::vec3(0.0f, 5.0f, 50.0f));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    CoordinateAxes ca(&CameraController::camera);
    FPSCounter fc;
    Camera *cam = &CameraController::camera;

    Shader plaentShader("shaders/Instancing/model.vs", "shaders/Instancing/model.frag");
    Shader rockShader("shaders/Instancing/rocks_instancing.vs", "shaders/Instancing/rocks_instancing.frag");

    Model planet((GLchar*)"textures/planet/planet.obj");
    planet.setCamera(cam);
    planet.setShader(&plaentShader);
    glm::mat4 planetModel;
    planetModel = glm::translate(planetModel, glm::vec3(0.0f, -5.0f, 0.0f));
    planetModel = glm::scale(planetModel, glm::vec3(4.0f, 4.0f, 4.0f));
    planet.modelMat = planetModel;

    Model rock((GLchar*)"textures/rock/rock.obj");

    //随机生产ROCK_AMOUNT个model矩阵
    glm::mat4 modelMatrices[ROCK_AMOUNT];
    srand(glfwGetTime());
    GLfloat radius = 50.0f;
    GLfloat offset = 5.0f;
    for(int i = 0; i < ROCK_AMOUNT; ++i){
        glm::mat4 model;
        // 1. Translation: displace along circle with 'radius' in range [-offset, offset]
        GLfloat angle = (GLfloat)i / (GLfloat)ROCK_AMOUNT * 360.0f;
        GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat x = sin(angle) * radius + displacement;
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat y = displacement * 0.4f; // y value has smaller displacement
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2. Scale: Scale between 0.05 and 0.25f
        GLfloat scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
        // 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
        GLfloat rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        // 4. Now add to list of matrices
        modelMatrices[i] = model;
    }
    //申请UBO 存放matrix
    GLuint UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(modelMatrices), &modelMatrices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //给Shader绑定UBO映射
    glUniformBlockBinding(rockShader.programID, glGetUniformBlockIndex(rockShader.programID, "UBO"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        // 绘制行星
        planet.draw();

        //绘制陨石
        rockShader.use();
        glUniformMatrix4fv(glGetUniformLocation(rockShader.programID,"view"),1,GL_FALSE,cam->getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(rockShader.programID,"projection"),1,GL_FALSE,cam->getProjectionMatrixVal());
        glBindVertexArray(rock.meshes[0].VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, rock.meshes[0].vertices.size(), ROCK_AMOUNT);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLfloat quadVertices[] = {
    //  ---位置---           ------颜色-------
    -0.005f,  0.005f, 0.00f,  1.0f, 0.0f, 0.0f,
     0.005f, -0.005f, 0.00f,  0.0f, 1.0f, 0.0f,
    -0.005f, -0.005f, 0.00f,  0.0f, 0.0f, 1.0f,

    -0.005f,  0.005f, 0.00f,  1.0f, 0.0f, 0.0f,
     0.005f, -0.005f, 0.00f,  0.0f, 1.0f, 0.0f,
     0.005f,  0.005f, 0.00f,  0.0f, 1.0f, 1.0f
};

#define NUM 100
#define OFFSET 0.01f
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1200
// 绘制N个平面四边形
void exercise_without_instancing(){
    GLFWwindow *window = initWindow("NOT_Instancing", WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSwapInterval(0);
    showEnviroment();

    CameraController::bindControl(window);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/Instancing/quads_not_instancing.vs", "shaders/Instancing/quads_not_instancing.frag");

    Object quadMother(quadVertices, 6, POSITIONS_COLORS, GL_TRIANGLES);
    Object *quads[NUM * NUM];
    int qPos=0;

    for(int y = -NUM; y < NUM; y+=2){
        for(int x = -NUM; x < NUM; x+=2){
            quads[qPos] = quadMother.clone();
            quads[qPos]->moveTo(glm::vec3(x / float(NUM) + OFFSET, y / float(NUM) + OFFSET, 0.0f));
            quads[qPos]->setShader(&shader);
            quads[qPos]->setCamera(nullptr);
            ++qPos;
        }
    }

    FPSCounter fc;
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        for(int i = 0; i < NUM*NUM; ++i){
            quads[i]->draw();
        }

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// 绘制N个平面四边形
void exercise_with_instancing_by_UBO(){
    GLFWwindow *window = initWindow("Instancing", WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSwapInterval(0);
    showEnviroment();

    CameraController::bindControl(window);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/Instancing/quads_instancing.vs", "shaders/Instancing/quads_instancing.frag");

    //为了配合UBO std140 布局，采取vec4
    GLfloat offsets[4 * NUM * NUM];
    int oPos=0;
    for(int y = -NUM; y < NUM; y+=2){
        for(int x = -NUM; x < NUM; x+=2){
            offsets[oPos] = x / float(NUM) + OFFSET;
            offsets[oPos + 1] = y / float(NUM) + OFFSET;
            offsets[oPos + 2] = 0.0f;
            offsets[oPos + 4] = 0.0f;
            oPos += 4;
        }
    }
    //申请UBO 存放offset
    GLuint UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(offsets), offsets, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //给Shader绑定UBO映射
    glUniformBlockBinding(shader.programID, glGetUniformBlockIndex(shader.programID, "UBO"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
    //物体
    Object quad(quadVertices, 6, POSITIONS_COLORS, GL_TRIANGLES);

    FPSCounter fc;
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        shader.use();
        glBindVertexArray(quad.VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM * NUM);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        fc.update();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif //INSTANCING_HPP
