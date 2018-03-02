/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef ADVANCED_DATA_CPP
#define ADVANCED_DATA_CPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace AdvancedData{

GLfloat cubeVertices1[] = {
    // Positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

//顶点信息
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
/// 写入Vertex Buffer的几种方法
void tutorial(){
    GLFWwindow *window = initWindow("AdvancedData", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    Camera* pCamera = CameraController::getCamera();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CameraController::bindControl(window);
    glEnable(GL_DEPTH_TEST);
    CoordinateAxes ca(pCamera);
    //
    Shader shader("Resources/Shaders/AdvancedData/scene2.vs","Resources/Shaders/AdvancedData/scene2.frag");
    //
    Texture tex0("Resources/Textures/container2.png", GL_BGRA, GL_RGBA);
    //
    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        /***************
        //方法1: 直接在BindVBO之后调用glBufferData写入
        glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices1),cubeVertices1,GL_STATIC_DRAW);
        ***************/

        /**************
        //方法2: glBufferData的数据指针置空,然后多次调用glBufferSubData。glBufferData需要预留足够位置，不然会报invilid value错误。
        glBufferData(GL_ARRAY_BUFFER,200*sizeof(GL_FLOAT),nullptr,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0,11*3*sizeof(GL_FLOAT),cubeVertices1);
        glBufferSubData(GL_ARRAY_BUFFER,11*3*sizeof(GL_FLOAT),25*3*sizeof(GL_FLOAT),cubeVertices1+(11*3));
        **************/

        /**************
        //方法3: 和方法2类似，使用glMapBuffer映射内存到一个void*指针，然后对该指针指向地址开始写。 若申请的空间不够，有可能会内存泄漏。
        glBufferData(GL_ARRAY_BUFFER,10*sizeof(GL_FLOAT),nullptr,GL_STATIC_DRAW); //只申请10*4B
        void *ptr=glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        memcpy(ptr,cubeVertices1,sizeof(cubeVertices1));                        //复制了36*3*4B
        glUnmapBuffer(GL_ARRAY_BUFFER);
        **************/

        ///使用glBufferSubData 分批导入顶点位置，法线和纹理坐标。存储形式为PPP...NNN...TTT。而以往的储存形式为PNTPNTPNT...
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

    ///我们来拷贝一个物体吧: 复制之前必须用glBufferData+null来申请空间
    GLuint VAO2,VBO2;
    glGenVertexArrays(1,&VAO2);
    glGenBuffers(1,&VBO2);
    //拷贝
    glBindBuffer(GL_COPY_READ_BUFFER, VBO);
    glBindBuffer(GL_COPY_WRITE_BUFFER, VBO2);
    //申请空间
    glBufferData(GL_COPY_WRITE_BUFFER,sizeof(cubePositions)+sizeof(cubeNormals)+sizeof(cubeTexCoords),nullptr,GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,0,sizeof(cubePositions)+sizeof(cubeNormals)+sizeof(cubeTexCoords));
    //在VAO中绑定
    glBindVertexArray(VAO2);
        glBindBuffer(GL_ARRAY_BUFFER,VBO2);
        //位置
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
        glEnableVertexAttribArray(0);
        //法线
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)(sizeof(cubePositions)));
        glEnableVertexAttribArray(1);
        //纹理坐标
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),(GLvoid*)(sizeof(cubePositions)+sizeof(cubeNormals)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glm::mat4 model=glm::mat4();
    model=glm::translate(model,glm::vec3(2,0,0));
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        CameraController::update();

        ca.draw();

        tex0.use(0);
        shader.use();
        glUniformMatrix4fv(glGetUniformLocation(shader.programID,"view"),
                           1, GL_FALSE, pCamera->getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(shader.programID,"projection"),
                           1, GL_FALSE, pCamera->getProjectionMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(shader.programID,"model"),
                           1, GL_FALSE, glm::value_ptr(glm::mat4()));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);

        glBindVertexArray(VAO2);
        glUniformMatrix4fv(glGetUniformLocation(shader.programID,"model"),1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
}
}

#endif // ADVANCED_DATA_HPP
