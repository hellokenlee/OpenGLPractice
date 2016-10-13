/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef CAMERAS_HPP
#define CAMERAS_HPP
namespace Cameras{
//窗口宽高
const float ScreenHeight=600.0;
const float ScreenWidth=800.0;
//顶点信息前置声明
extern GLfloat vertices[36*5];
//位置信息前置声明
extern glm::vec3 cubePositions[10];
//按键回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//位置更新函数
void doMovement();
//鼠标回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//滑轮回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//摄像机信息
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);      //摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   //摄像机前方(摄像机永远注视着摄像机的以摄像机为原点 0,0,-1的 位置)
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       //摄像机的上方向(喂！不许歪着头看东西!)
//FPS计数器
FPSCounter fc;
//按键记录
bool keys[1024]={0};
//鼠标灵敏度
double sensitivity = 0.05f;
//鼠标相对转动角
GLfloat pitch=0,yaw= -90.0f;
GLfloat fov=45.0;
//上次鼠标位置
double lastX,lastY;


GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
//教程实现： 实现一个FPS视角摄像机
void tutorial(){
    //窗口初始化
    GLFWwindow *window=initWindow("Camera",ScreenWidth,ScreenHeight,key_callback,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);
    //关闭垂直同步
    glfwSwapInterval(0);
    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示环境
    showEnviroment();
    //创建Shader
    Shader shaderProgram("shaders/CoordinateSystem/shader.vs","shaders/CoordinateSystem/shader.frag");
    //读入数据
    GLuint VBO,VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=Textures::loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);
    GLuint tex1=Textures::loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_REPEAT,GL_LINEAR);
    //初始化矩阵
    //模型矩阵(局部coord -> 世界coord)
    glm::mat4 model;
    //观察矩阵(世界coord -> 观察coord)
    glm::mat4 view;
    //投影矩阵(观察coord -> 裁剪coord)
    glm::mat4 projection;
    projection=glm::perspective(45.0f,ScreenWidth/ScreenHeight,0.1f,100.0f);
    //3个矩阵的Shader uniform位置
    GLuint modelPos=glGetUniformLocation(shaderProgram.programID,"model");
    GLuint viewPos=glGetUniformLocation(shaderProgram.programID,"view");
    GLuint projPos=glGetUniformLocation(shaderProgram.programID,"proj");
    //初始化鼠标位置
    glfwGetCursorPos(window,&lastX,&lastY);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        doMovement();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //设置矩阵的值
        view = glm::lookAt(
            cameraPos,//摄像机位置
            cameraPos+cameraFront,//摄像机需要看到的位置//正前方
            cameraUp//摄像机的正上方（需要指定上方的原因是，在同一个位置看同一个地方，头可以歪）
        );
        projection=glm::perspective(fov, (GLfloat)ScreenWidth/(GLfloat)ScreenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(viewPos,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projPos,1,GL_FALSE,glm::value_ptr(projection));
        //绑定纹理1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);
        //绑定纹理2
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        for(GLuint i=0;i<10;i++){
          model=glm::mat4();
          model = glm::translate(model, cubePositions[i]);
          GLfloat angle = 20.0f * i*(GLfloat)glfwGetTime();
          model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
          glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));

          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        fc.update();
    }//GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return;
}

//练习1
void doMovementEx1(){
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    GLfloat cameraSpeed = 5.0f * deltaTime;
    glm::vec3 playerFront=cameraFront;
    //playerFront.y=0;
    //glm::normalize(playerFront);
    if(keys[GLFW_KEY_W])
        //往前
        cameraPos+=cameraSpeed*playerFront;
    if(keys[GLFW_KEY_S])
        //往后
        cameraPos-=cameraSpeed*playerFront;
    if(keys[GLFW_KEY_A])
        //往左：右边方向通过正前方 叉乘 正上方获得，往左则反方向
        cameraPos-=glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
    if(keys[GLFW_KEY_D])
        cameraPos+=glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
    cameraPos.y=0;
}
void exercise1(){
    //窗口初始化
    GLFWwindow *window=initWindow("Camera-EX1",ScreenWidth,ScreenHeight,key_callback,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);
    //关闭垂直同步
    glfwSwapInterval(0);
    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示环境
    showEnviroment();
    //创建Shader
    Shader shaderProgram("shaders/CoordinateSystem/shader.vs","shaders/CoordinateSystem/shader.frag");
    //读入数据
    GLuint VBO,VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=Textures::loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);
    GLuint tex1=Textures::loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_REPEAT,GL_LINEAR);
    //初始化矩阵
    //模型矩阵(局部coord -> 世界coord)
    glm::mat4 model;
    //观察矩阵(世界coord -> 观察coord)
    glm::mat4 view;
    //投影矩阵(观察coord -> 裁剪coord)
    glm::mat4 projection;
    projection=glm::perspective(45.0f,ScreenWidth/ScreenHeight,0.1f,100.0f);
    //3个矩阵的Shader uniform位置
    GLuint modelPos=glGetUniformLocation(shaderProgram.programID,"model");
    GLuint viewPos=glGetUniformLocation(shaderProgram.programID,"view");
    GLuint projPos=glGetUniformLocation(shaderProgram.programID,"proj");
    //初始化鼠标位置
    glfwGetCursorPos(window,&lastX,&lastY);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        doMovementEx1();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //设置矩阵的值
        view = glm::lookAt(
            cameraPos,//摄像机位置
            cameraPos+cameraFront,//摄像机需要看到的位置//正前方
            cameraUp//摄像机的正上方（需要指定上方的原因是，在同一个位置看同一个地方，头可以歪）
        );
        projection=glm::perspective(fov, (GLfloat)ScreenWidth/(GLfloat)ScreenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(viewPos,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projPos,1,GL_FALSE,glm::value_ptr(projection));
        //绑定纹理1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);
        //绑定纹理2
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        for(GLuint i=0;i<10;i++){
          model=glm::mat4();
          model = glm::translate(model, cubePositions[i]);
          GLfloat angle = 20.0f * i*(GLfloat)glfwGetTime();
          model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
          glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));
          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        fc.update();
    }//GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return;
}

//处理鼠标位移
void mouse_callback(GLFWwindow* window, double nowX, double nowY){
    GLfloat deltaX=nowX-lastX,deltaY=lastY-nowY;
    lastX=nowX;
    lastY=nowY;
    deltaX *= sensitivity;
    deltaY *= sensitivity;
    yaw+=deltaX;
    pitch+=deltaY;
    if(pitch>89.5f)
        pitch=89.5f;
    else if(pitch<-89.5)
        pitch=-89.5;
    //cout<<pitch<<" , "<<yaw<<endl;
    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront=glm::normalize(cameraFront);
    //cout<<cameraFront.x<<" , "<<cameraFront.y<<" , "<<cameraFront.z<<endl;
}
//处理按键
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}
void doMovement(){
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if(keys[GLFW_KEY_W])
        //往前
        cameraPos+=cameraSpeed*cameraFront;
    if(keys[GLFW_KEY_S])
        //往后
        cameraPos-=cameraSpeed*cameraFront;
    if(keys[GLFW_KEY_A])
        //往左：右边方向通过正前方 叉乘 正上方获得，往左则反方向
        cameraPos-=glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
    if(keys[GLFW_KEY_D])
        cameraPos+=glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}
//位置信息
glm::vec3 cubePositions[10] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
//顶点信息
GLfloat vertices[36*5] = {
    //position            //texture
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


};      //  namespace Camera
#endif  //  CAMERA_HPP
