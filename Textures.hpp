/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TEXTURES_HPP
#define TEXTURES_HPP

namespace Textures{
//从图片文件生产纹理
GLuint loadTextureFromImage(const char* filename){
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
    fif=FreeImage_GetFileType(filename);
    if(fif==FIF_UNKNOWN)
        fif=FreeImage_GetFIFFromFilename(filename);
    if(fif==FIF_UNKNOWN){
        cout<<"ERROR: Unkown image file"<<endl;
        exit(-1);
    }
    //载入图片
    image=FreeImage_Load(fif,filename);
    //获取数据
    bits=FreeImage_GetBits(image);
    width=FreeImage_GetWidth(image);
    height=FreeImage_GetHeight(image);
    //生产纹理
    glGenTextures(1,&texID);
    //指定纹理类型
    glBindTexture(GL_TEXTURE_2D,texID);
    //载入图片数据到纹理
    glTexImage2D(GL_TEXTURE,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,bits);
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
//教程实现
void tutorial(){
    //创建窗口
    GLFWwindow *window=initWindow("Shaders",800,600);
    showEnviroment();
    //创建Shader
    Shaders::Shader shaderProgram("","");
    //读入数据
    GLuint VBO,VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//color
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    //设置纹理
    GLuint tex=loadTextureFromImage("");
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);

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
