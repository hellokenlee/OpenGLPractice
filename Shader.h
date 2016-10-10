/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

//  
// 	Shader类: 读取，编译，链接GLSL
//

class Shader{
public:
	//opengl 中的shader程序ID
	GLuint shaderID;
public:
	//构造函数(VextexShader's GLSL filepath,FragmentShader's GLSL filepath)
	Shader(const GLchar* vShaderFilePath,const GLchar* fShaderFilePath,const bool debug=true);
	//使用该Shader程序
	void use();
private:
	//禁止默认构造函数
	Shader();
};


#endif	//	SHADER_H