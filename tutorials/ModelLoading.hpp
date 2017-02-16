/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef MODEL_LOADING_HPP
#define MODEL_LOADING_HPP
namespace ModelLoading{
//顶点信息前置声明
extern GLfloat cubeVertices[8*3];
extern GLuint cubeIndices[36];


//Model类的单元测试
void modelTest(){
    GLFWwindow *window=initWindow("ModelLoadingTest",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);

}

//显示程序
void tutorial(){


}

//顶点信息
GLfloat cubeVertices[8*3]={
     0.5f,  0.5f,  0.5f, //0
     0.5f,  0.5f, -0.5f, //1
     0.5f, -0.5f,  0.5f, //2
     0.5f, -0.5f, -0.5f, //3
    -0.5f,  0.5f,  0.5f, //4
    -0.5f,  0.5f, -0.5f, //5
    -0.5f, -0.5f,  0.5f, //6
    -0.5f, -0.5f, -0.5f  //7
};
//顶点索引
GLuint cubeIndices[3*2*6]={
    7,3,1,
    1,5,7,

    6,2,0,
    0,4,6,

    4,5,7,
    7,6,4,

    0,1,3,
    3,2,0,

    7,3,2,
    2,6,7,

    5,1,0,
    0,4,5
};
};
#endif // MODEL_LOADING_HPP
