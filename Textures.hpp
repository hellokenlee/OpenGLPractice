/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TEXTURES_HPP
#define TEXTURES_HPP

namespace Textures{
//从图片文件生产纹理
GLuint loadTextureFromImage(const char* filename,GLenum texformat,GLenum imageformat){
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
    //载入图片数据到纹理
    glTexImage2D(GL_TEXTURE_2D,0,texformat,width,height,0,imageformat,GL_UNSIGNED_BYTE,bits);
    glGenerateMipmap(GL_TEXTURE_2D);
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
//教程实现
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("Shaders",800,600);
    showEnviroment();
    //创建Shader
    Shaders::Shader shaderProgram("shaders/Textures/shader.vs","shaders/Textures/shader.frag");
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
    GLuint tex2=loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA);
    GLuint tex1=loadTextureFromImage("textures/container.jpg",GL_RGB,GL_BGR);
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
//练习1
void exercise1(){

}
//练习2
void exercise2(){

}
//练习3
void exercise3(){

}


};      //namespace Textures
#endif  //TEXTURE_HPP
