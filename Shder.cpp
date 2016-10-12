/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/

#include "Shader.h"

Shader::Shader(const GLchar* vShaderSourcePath,const GLchar* fShaderSourcePath,const bool debug){
    std::string vShaderSource;
    std::string fShaderSource;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        //读入Shader文件
        vShaderFile.open(vShaderSourcePath);
        fShaderFile.open(fShaderSourcePath);
        std::stringstream vShaderStream,fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vShaderSource=vShaderStream.str();
        fShaderSource=fShaderStream.str();
    } catch(std::ifstream::failure e) {
        //处理异常
        std::cout<<"Error::Shader(const char*,const char*)::Fail to find shader source file!"<<std::endl;
        std::cout<<e.what()<<std::endl;
        this->programID=0;
        return;
    }
    const GLchar* vShaderCode=vShaderSource.c_str();
    const GLchar* fShaderCode=fShaderSource.c_str();
    //编译Shader
    GLuint vShader,fShader;
    vShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader,1,&vShaderCode,nullptr);
    glCompileShader(vShader);
    if(debug)
        compileCheck("VertexShader",vShader);
    fShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader,1,&fShaderCode,nullptr);
    glCompileShader(fShader);
    if(debug)
        compileCheck("FragmentShader",fShader);
    //链接Shader程序
    this->programID=glCreateProgram();
    glAttachShader(this->programID,vShader);
    glAttachShader(this->programID,fShader);
    glLinkProgram(this->programID);
    if(debug)
        linkCheck();
}

Shader::~Shader(){
    //释放program空间,包括attached的shaders
    glDeleteProgram(this->programID);
}

void Shader::use(){
    glUseProgram(this->programID);
}

//检查GLSL编译情况
void Shader::compileCheck(const char* sName,GLuint shaderID){
    GLint isSuccess;
    glGetShaderiv(shaderID,GL_COMPILE_STATUS,&isSuccess);
    if(!isSuccess){
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID,512,nullptr,infoLog);
        std::cout<<"ERROR: Comlile "<<sName<<" Shader Error:"<<std::endl<<infoLog<<std::endl;
        exit(-1);
    }
}
//检查多Shader链接情况
void Shader::linkCheck(){
    GLint isSuccess;
    glGetProgramiv(this->programID,GL_LINK_STATUS,&isSuccess);
    if(!isSuccess){
        GLchar infoLog[512];
        glGetProgramInfoLog(programID,512,nullptr,infoLog);
        std::cout<<"ERROR: Link Program Error:"<<std::endl<<infoLog<<std::endl;
        exit(-1);
    }
}
