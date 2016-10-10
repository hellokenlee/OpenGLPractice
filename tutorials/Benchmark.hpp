/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#define MAX_VERTICES_NUM 100000
namespace Benchmark{


GLfloat vertices[MAX_VERTICES_NUM];
int vNums;

float texCoord[]={
    0.0,0.0,
    1.0,0.0,
    0.0,1.0
};

void readVerticesFromFile(const char* filename){
    vNums=0;
    int pos=0;
    FILE *file=fopen(filename,"r+");
    while(fscanf(file,"%f%f%f%f%f%f%f%f",&vertices[pos+0],&vertices[pos+1],&vertices[pos+2]
                                        ,&vertices[pos+3],&vertices[pos+4],&vertices[pos+5]
                                        ,&vertices[pos+6],&vertices[pos+7])!=EOF){
        pos+=8;
        ++vNums;
    }
    printf("Finished load %d vertices data.\n",vNums);
    fclose(file);
//    cout<<vertices[0]<<" "<<vertices[1]<<" "<<vertices[2]<<endl;
//    cout<<vertices[3]<<" "<<vertices[4]<<" "<<vertices[5]<<endl;
//    cout<<vertices[6]<<" "<<vertices[7]<<endl;
    return;
}

void oldOpenGLTest(){
    //读取顶点信息
    readVerticesFromFile("3vertices10000.txt");
    //FPS计算
    double lastTime=glfwGetTime(),currentTime=glfwGetTime();
    int nbFrames=0;
    //初始化
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(800,600,"Old OpenGL",nullptr,nullptr);
    glfwMakeContextCurrent(window);
    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);
    glEnable(GL_TEXTURE_2D);
    glfwSwapInterval(0);
    showEnviroment();
    //绑定素材
    GLuint tex=Textures::loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //draw
        glBindTexture(GL_TEXTURE_2D,tex);
        glBegin(GL_TRIANGLES);
            for(int i=0;i<vNums;++i){
                //glColor3f(1,0,0);
                glColor3f(vertices[8*i+3],vertices[8*i+4],vertices[8*i+5]);
                glTexCoord2f(vertices[8*i+6],vertices[8*i+7]);
                glVertex3f(vertices[8*i+0],vertices[8*i+1],vertices[8*i+2]);
            }
        glEnd();
        glfwSwapBuffers(window);
        //Count FPS
        currentTime=glfwGetTime();
        ++nbFrames;
        if(currentTime-lastTime >=1.0){
            printf("%f ms/frame\n",1000.0/double(nbFrames));
            nbFrames=0;
            lastTime+=1.0;
        }
    }
    glfwTerminate();
}


void coreprofileTest(){
    //读取顶点信息
    readVerticesFromFile("3vertices10000.txt");
    //FPS计算
    double lastTime=glfwGetTime(),currentTime=glfwGetTime();
    int nbFrames=0,cnt=3*vNums;
    //初始化
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(800,600,"New OpenGL",nullptr,nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){
        cout<<"ERROR: Fail to init GLEW"<<endl;
        exit(-1);
    }
    glGetError();//忽略由glew引起的INVALID_ENUM错误
    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);
    glEnable(GL_TEXTURE_2D);
    glfwSwapInterval(0);

    showEnviroment();
    //绑定素材
    GLuint tex=Textures::loadTextureFromImage("textures/face.png",GL_RGBA,GL_BGRA,GL_REPEAT,GL_LINEAR);
    //Shader绑定
    Shaders::Shader shaderProgram("shader.vs","shader.frag");

    GLuint VBO,VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        //背景色
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //
        glBindVertexArray(VAO);
        shaderProgram.use();
        glBindTexture(GL_TEXTURE_2D,tex);
        //绘制
        glDrawArrays(GL_TRIANGLES,0,cnt);
        //置换双缓冲
        glfwSwapBuffers(window);
        //Count FPS
        currentTime=glfwGetTime();
        ++nbFrames;
        if(currentTime-lastTime >=1.0){
            printf("%f ms/frame\n",1000.0/double(nbFrames));
            nbFrames=0;
            lastTime+=1.0;
        }
    }
    glfwTerminate();
}
};
#endif // BENCHMARK_HPP
