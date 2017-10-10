/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#define MAX_VERTICES_NUM 100000
//GLM
#include "../wmdge/glm/glm.hpp"
#include "../wmdge/glm/gtc/matrix_transform.hpp"
#include "../wmdge/glm/gtc/type_ptr.hpp"
namespace Benchmark{


GLfloat vertices[MAX_VERTICES_NUM];
int vNums;

float texCoord[]={
    0.0,0.0,
    1.0,0.0,
    0.0,1.0
};

bool readVerticesFromFile(const char* filename){
    vNums=0;
    int pos=0;
    FILE *file=fopen(filename,"r+");
    if(file==nullptr)
        return false;
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
    return true;
}

void oldOpenGLTest(){
    //读取顶点信息
    if(!readVerticesFromFile("3vertices10000.txt")){
        cout<<"ERROR: Fail to read file"<<endl;
        return;
    }
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
    unsigned int texMid=0;
    TextureManager *texManager=TextureManager::getManager();
    if(!texManager->loadTexture("textures/face.png",texMid,GL_RGBA,GL_BGRA)){
        cout<<"load texture failed"<<endl;
        return ;
    }

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //draw
        texManager->bindTexture(texMid);
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
    if(!readVerticesFromFile("3vertices10000.txt")){
        cout<<"ERROR: Fail to read file"<<endl;
        return;
    }
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
    unsigned int texMid=0;
    TextureManager *texManager=TextureManager::getManager();
    if(!texManager->loadTexture("textures/face.png",texMid,GL_RGBA,GL_BGRA)){
        cout<<"load texture failed"<<endl;
        return ;
    }
    //Shader绑定
    Shader shaderProgram("shaders/Benchmark/shader.vs","shaders/Benchmark/shader.frag");

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
        texManager->bindTexture(texMid);
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

//顶点信息
GLfloat verticesTest[36*5] = {
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

//测试台
void test(){
    FPSCounter fc;
    //窗口初始化
    GLFWwindow *window=initWindow("Test",800,600);
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
        glBufferData(GL_ARRAY_BUFFER,sizeof(verticesTest),verticesTest,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);                  //pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));//tex coord
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    //设置纹理
    unsigned int texs[]={0,1};
    TextureManager *texManager=TextureManager::getManager();
    if(!texManager->loadTexture("textures/face.png",texs[1],GL_BGRA,GL_RGBA))
        return;
    if(!texManager->loadTexture("textures/container.jpg",texs[0]))
        return;
    //控制器绑定
    glfwSetKeyCallback(window,CameraController::keyCallback);
    glfwSetCursorPosCallback(window,CameraController::mouseCallback);
    glfwSetScrollCallback(window,CameraController::scrollCallback);
    //初始化矩阵
    glm::mat4 model;
    //3个矩阵的Shader uniform位置
    GLuint modelPos=glGetUniformLocation(shaderProgram.programID,"model");
    GLuint viewPos=glGetUniformLocation(shaderProgram.programID,"view");
    GLuint projPos=glGetUniformLocation(shaderProgram.programID,"proj");
    //
    CoordinateAxes ca(&CameraController::camera);
    //主循环
    while(!glfwWindowShouldClose(window)){
        //Keybord and mouse
        glfwPollEvents();
        CameraController::update();
        //BGC
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        ca.draw();
        shaderProgram.use();
        //摄像机更新
        glUniformMatrix4fv(viewPos,1,GL_FALSE,CameraController::camera.getViewMatrixVal());
        glUniformMatrix4fv(projPos,1,GL_FALSE,CameraController::camera.getProjectionMatrixVal());
        //绑定纹理1
        glActiveTexture(GL_TEXTURE0);
        texManager->bindTexture(texs[0]);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex1"),0);
        //绑定纹理2
        glActiveTexture(GL_TEXTURE1);
        texManager->bindTexture(texs[1]);
        glUniform1i(glGetUniformLocation(shaderProgram.programID,"tex2"),1);
        //Draw
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
};
#endif // BENCHMARK_HPP
