/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
//
// 	Shader类: 读取，编译，链接GLSL
//

class Shader{
public:
	//opengl 中的shader程序ID
	GLuint programID;
public:
	//构造函数(VextexShader's GLSL filepath,FragmentShader's GLSL filepath)
	Shader(const GLchar* vShaderFilePath,const GLchar* fShaderFilePath,const bool debug=true);
	//析构函数 释放Shader程序空间
	~Shader();
	//使用该Shader程序
	void use();
private:
	//禁止默认构造函数
	Shader(){};
	void compileCheck(const char* sName,GLuint shaderID);
    void linkCheck();
};


#endif	//	SHADER_H
