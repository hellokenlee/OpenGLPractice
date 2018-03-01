/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

#include "Utils.h"

//
// 	Shader类: 读取，编译，链接GLSL
//
class Shader {
public:
    // 构造函数：从文件构造
    Shader(const GLchar* vShaderFilePath, const GLchar* fShaderFilePath);
    // 析构函数 释放Shader程序空间
    ~Shader();
    // 增加可选着色器
    bool addOptionalShader(const GLchar* shaderFilePath, GLenum shaderType);
    // 使用该Shader程序
    void inline use() const{
        glUseProgram(programID);
    }
    // 从字符串中构造着色器
    static std::unique_ptr<Shader> createShaderFromSource(const GLchar* vShaderSource, const GLchar* fShaderSource);
    // 从文件中读取Shader代码
    static std::string readShaderFile(const GLchar* shaderFilePath);
public:
    // 着色器ID
    GLuint programID;
private:
    // 禁止拷贝
    Shader() {};
    Shader(const Shader& rhs);
    Shader& operator=(const Shader& rhs);
    //
    void compileCheck(const GLenum shaderType, GLuint shaderID);
    void linkCheck();
    void createShader(const GLchar* vShaderSource, const GLchar* fShaderSource);
    GLuint fShader, vShader;
};


#endif	//	SHADER_H
