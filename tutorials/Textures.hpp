/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TEXTURES_HPP
#define TEXTURES_HPP

namespace Textures{
//从图片文件生产纹理
GLuint loadTextureFromImage(const char* filename,GLenum texformat,GLenum imageformat,GLenum wrap,GLenum filter){
    //纹理ID
    GLuint texID;
    //图片格式
    FREE_IMAGE_FORMAT fif=FIF_UNKNOWN;
    //图片数据
    BYTE *bits(0);
    //图片指针
    FIBITMAP *image(0);
    //宽，高
    GLuint width,height;
    //检查格式
    fif=FreeImage_GetFileType(filename,0);
    if(fif==FIF_UNKNOWN)
        fif=FreeImage_GetFIFFromFilename(filename);
    if(fif==FIF_UNKNOWN){
        cout<<"ERROR::TEXTURE Unkown image file"<<endl;
        exit(-1);
    }
    //载入图片
    if(FreeImage_FIFSupportsReading(fif))
		image = FreeImage_Load(fif, filename);
    if(!image){
        cout<<"ERROR::TEXTURE Fail to load image"<<endl;
        exit(-1);
    }
    //获取数据
    bits=FreeImage_GetBits(image);
    width=FreeImage_GetWidth(image);
    height=FreeImage_GetHeight(image);
    if((bits == 0) || (width == 0) || (height == 0)){
        cout<<"ERROR::TEXTURE Fail to get image info"<<endl;
        exit(-1);
    }
    //生产纹理
    glGenTextures(1,&texID);
    //指定纹理类型
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    //载入图片数据到纹理
    glTexImage2D(GL_TEXTURE_2D,0,texformat,width,height,0,imageformat,GL_UNSIGNED_BYTE,bits);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //释放图片
    FreeImage_Unload(image);
    //恢复状态机状态
    glBindTexture(GL_TEXTURE_2D,0);
    return texID;
}
GLfloat vertices[] = {
    // Positions          // Colors           // Texture Coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left
};
GLuint indices[] = {
    0,1,3,
    1,2,3
};
//教程实现: 两张纹理叠加在一个矩形内
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("Textures",800,600,nullptr);
    showEnviroment();
    //创建Shader
    Shader shaderProgram("shaders/Textures/shader.vs","shaders/Textures/shader.frag");
    //读入数据
    GLuint VBO,VAO,EBO;
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);
    GLuint tex1=loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_REPEAT,GL_LINEAR);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return;
}

//练习1: 只更改fShader， 让笑脸朝向不同方向(沿xy翻转)
void exercise1(){
    //创建窗口
    GLFWwindow *window=initWindow("Textures-EX1",800,600,nullptr);
    showEnviroment();
    //创建Shader
    Shader shaderProgram("shaders/Textures/shader.vs","shaders/Textures/shaderEx1.frag");
    //读入数据
    GLuint VBO,VAO,EBO;
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);
    GLuint tex1=loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_REPEAT,GL_LINEAR);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return;
}

//练习2 让纹理坐标越界，用不同得WRAP方法处理2个纹理
void exercise2(){
    //创建窗口
    GLFWwindow *window=initWindow("Textures-EX2",800,600,nullptr);
    showEnviroment();
    //创建Shader
    Shader shaderProgram("shaders/Textures/shader.vs","shaders/Textures/shaderEx2.frag");
    //读入数据
    GLuint VBO,VAO,EBO;
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_MIRRORED_REPEAT,GL_LINEAR);
    GLuint tex1=loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_CLAMP_TO_EDGE,GL_LINEAR);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return;
}


//练习3  只显示纹理图片的最中心使其出现像素差异
GLfloat verticesEx3[] = {
    // Positions          // Colors           // Texture Coords  (Note that we changed them to 'zoom in' on our texture image)
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.53f, 0.53f,  // Top Right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.53f, 0.47f, // Bottom Right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.47f, 0.47f, // Bottom Left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.47f, 0.53f  // Top Left
};
void exercise3(){
    //创建窗口
    GLFWwindow *window=initWindow("Texture-EX3",800,600,nullptr);
    showEnviroment();
    //创建Shader
    Shader shaderProgram("shaders/Textures/shader.vs","shaders/Textures/shaderEx4.frag");
    //读入数据
    GLuint VBO,VAO,EBO;
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(verticesEx3),verticesEx3,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_NEAREST);
    GLuint tex1=loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_REPEAT,GL_NEAREST);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return;
}
//练习4
GLfloat alpha=0.5;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_UP && (action ==GLFW_REPEAT || action==GLFW_PRESS)){
        if(alpha<1){
            alpha+=0.02;
        }
    }
    if (key == GLFW_KEY_DOWN && (action ==GLFW_REPEAT || action==GLFW_PRESS)){
        if(alpha>0){
            alpha-=0.02;
        }
    }
}

//
void exercise4(){
    //FPS计算
    FPSCounter fc;
    //创建窗口
    GLFWwindow *window=initWindow("Texture-EX4",800,600,key_callback);
    //关闭垂直同步
    glfwSwapInterval(0);
    //显示环境
    showEnviroment();
    //创建Shader
    Shader shaderProgram("shaders/Textures/shader.vs","shaders/Textures/shaderEx4.frag");
    //读入数据
    GLuint VBO,VAO,EBO;
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex2=loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);
    GLuint tex1=loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR,GL_REPEAT,GL_LINEAR);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex2);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //按键改变值传入
        glUniform1f(glGetUniformLocation(shaderProgram.programID,"alpha"),alpha);
        //Draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        //Count FPS
        fc.update();
    }
    glfwTerminate();
    return;
}


};      //namespace Textures
#endif  //TEXTURE_HPP
