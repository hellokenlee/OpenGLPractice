/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/

#include "Shader.h"

//
Shader::Shader(const GLchar* vShaderFilePath, const GLchar* fShaderFilePath) {
    // 初始化
    this->programID = 0;
    const GLchar* vShaderCode;
    const GLchar* fShaderCode;
    std::string vShaderSource;
    std::string fShaderSource;
    // 获取着色器源代码
    vShaderSource = readShaderFile(vShaderFilePath);
    fShaderSource = readShaderFile(fShaderFilePath);
    vShaderCode = vShaderSource.c_str();
    fShaderCode = fShaderSource.c_str();
    //
    createShader(vShaderCode, fShaderCode);
}

std::unique_ptr<Shader> Shader::createShaderFromSource(const GLchar* vShaderSource, const GLchar* fShaderSource) {
    Shader* res = new Shader();
    res->createShader(vShaderSource, fShaderSource);
    return std::unique_ptr<Shader>(res);
}


void Shader::createShader(const GLchar* vShaderSource, const GLchar* fShaderSource){
    // 编译Shader
    vShader = 0;
    fShader = 0;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSource, nullptr);
    glCompileShader(vShader);
    // 输出错误
    dCall(compileCheck(GL_VERTEX_SHADER, vShader));
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSource, nullptr);
    glCompileShader(fShader);
    // 输出错误
    dCall(compileCheck(GL_FRAGMENT_SHADER, fShader));
    // 链接Shader程序
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vShader);
    glAttachShader(this->programID, fShader);
    glLinkProgram(this->programID);
    //
    dCall(linkCheck());
}

// 从文件读入
std::string Shader::readShaderFile(const char* shaderFilePath) {
    //
    checkFileExist(shaderFilePath);
    //
    std::string shaderSourceString;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    //
    try {
        //从文件读入
        shaderFile.open(shaderFilePath);
        std::stringstream shaderSourceStream;
        shaderSourceStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderSourceString = shaderSourceStream.str();
    } catch(std::ifstream::failure e) {
        //处理异常
        dLog("[Error] : Fail to read shader source file!\n");
        dLog("%s\n\n", e.what());
        return "";
    }
    return shaderSourceString;
}

// 析构函数
Shader::~Shader() {
    //释放program空间,包括attached的shaders
    if(programID != 0)
        glDeleteProgram(this->programID);
}

//检查GLSL编译情况
void Shader::compileCheck(const GLenum shaderType, GLuint shaderID) {
    GLint isSuccess;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isSuccess);
    std::string shaderTypeName;
    switch(shaderType){
        case GL_VERTEX_SHADER: {
            shaderTypeName = "Vertex Shader";
            break;
        }
        case GL_TESS_CONTROL_SHADER: {
            shaderTypeName = "Tessellation Control Shader";
            break;
        }
        case GL_TESS_EVALUATION_SHADER: {
            shaderTypeName = "Tessellation Evaluation Shader";
            break;
        }
        case GL_GEOMETRY_SHADER: {
            shaderTypeName = "Geometry Shader";
            break;
        }
        case GL_FRAGMENT_SHADER: {
            shaderTypeName = "Fragment Shader";
            break;
        }
        default: shaderTypeName = "Unknown Shader";
    }
    if(!isSuccess) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        dLog("[ERROR]: Can not comlile %s:\n%s\n\n", shaderTypeName.c_str(), infoLog);
    }
}
//检查多Shader链接情况
void Shader::linkCheck() {
    GLint isSuccess;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &isSuccess);
    if(!isSuccess) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        ddLog("[ERROR]: Link Program Error:\n%s\n\n", infoLog);
    }
}

//
bool Shader::addOptionalShader(const GLchar* shaderFilePath, GLenum shaderType) {
    std::string shaderSourceString = readShaderFile(shaderFilePath);
    GLuint shader = glCreateShader(shaderType);
    const GLchar *shaderCode = shaderSourceString.c_str();
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    compileCheck(shaderType, shader);

    glAttachShader(this->programID, shader);
    glLinkProgram(this->programID);
    linkCheck();
    return true;
}
