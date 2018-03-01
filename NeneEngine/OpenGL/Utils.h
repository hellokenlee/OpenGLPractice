/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef UTILS_H
#define UTILS_H

/// Commons Headers
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
//
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <assert.h>

#define DEBUG
//#define DDEBUG
//	调试
#ifdef DEBUG
#define checkError() _checkError(__FILE__, __FUNCTION__, __LINE__)
#define checkFileExist(filePath) _checkFileExist(filePath)
#define dLog(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define dLogIf(expr, fmt, ...) if(expr) printf(fmt, ##__VA_ARGS__)
#define dCall(func) func
#else
#define checkError()
#define checkFileExist(filePath)
#define dLog(fmt, ...)
#define dLogIf(expr, fmt, ...)
#define dCall()
#endif //DEBUG

#ifdef DDEBUG
#define ddLog(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define ddLog(fmt, ...)
#endif // DDEBUG

// 创建窗口
GLFWwindow* initWindow(const char* window_name, int w, int h, int glMajor = 3, int glMinor = 3);

// 显示环境
void showEnviroment();

// 检查错误
void _checkError(const char* fileName, const char* func, const int line);

// 检查文件存在
void _checkFileExist(const char* filepath);


#endif
