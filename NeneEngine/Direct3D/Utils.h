/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <string>
#include <io.h>
#include <stdarg.h>
#include "Types.h"

//	调试宏
#ifdef DEBUG
#define checkResult(hr, errorStr, ...) Utils::_checkResult(__FILE__, __FUNCTION__, __LINE__, hr, errorStr, ##__VA_ARGS__)
#define checkError() Utils::_checkError(__FILE__, __FUNCTION__, __LINE__)
#define checkFileExist(filePath) Utils::_checkFileExist(filePath)
#define dLog(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define dLogIf(expr, fmt, ...) if(expr) printf(fmt, ##__VA_ARGS__)
#else
#define checkResult(hr, errorStr)  
#define checkError()  
#define checkFileExist(filePath)  
#define dLog(fmt, ...)   
#define dLogIf(expr, fmt, ...)    
#endif

//
//  用于管理窗口，渲染设备，上下文的静态类
//

///!TODO: 把多个RenderContext(如：ViewPorts, DepthStencilBuffer... 抽象成一个栈, multipass render的时候可以做环境保存)
class Utils {
public:
	// 初始化
	static void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
	// 创建窗口
	static HWND createWindow(const char* windowName, int width = 800, int height = 600);
	// 处理消息
	static void pollEvents();
	// 初始化D3D
	static void initDirectX();
	// 清理D3D
	static void cleanDirectX();
	// 设置背景色
	static void clearColor(FLOAT r, FLOAT g, FLOAT b);
	// 清理深度/目标缓冲
	static void clear();
	// 交换缓冲
	static void swapBuffer();
	// debug函数
	static void _checkError(const char* filename, const char* func, const int line);
	static void _checkResult(const char* filename, const char* func, const int line, HRESULT hr, const char* errorStr, ...);
	static void _checkFileExist(const WCHAR* filename);
	//
	static void setPolygonMode(PolygonMode mode);
	static inline PolygonMode getPolygonMode() {
		return nowPolygonMode;
	}
	// 返回窗口是否被关闭
	static inline bool windowShouldClose() {
		return isWindowShouldClose;
	}
	// 获取当前的渲染设备
	static inline ID3D11Device* getDevice() {
		return pDevice;
	}
	// 获取当前渲染上下文
	static inline ID3D11DeviceContext* getContext() {
		return pDeviceContext;
	}
	// 获取窗口相关信息
	static inline int getScreenWidth() {
		return screenWidth;
	}
	static inline int getScreenHeight() {
		return screenHeight;
	}
	static inline HWND getHWND() {
		return hWnd;
	}
public:
	static ID3D11RasterizerState* polygonModes[3];
	static PolygonMode nowPolygonMode;
private:
	static bool initDeviceAndSwapChain();
	static bool initDepthBuffer();
	static bool initRenderTarget();
public:
	// Windows事件处理
	static LRESULT CALLBACK defaultWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 默认Windows Class 名字
	static const char defaultClassName[];
	// 创建的窗口句柄
	static HWND hWnd;
	// 当前消息变量
	static MSG msg;
	// 标志窗口是否应该退出
	static bool isWindowShouldClose;
	// 应用程序当前实例句柄
	static HINSTANCE hInstance;
	// 应用程序上一个实例句柄
	static HINSTANCE hPrevInstance;
	// 使用命令行打开的附加参数字符串
	static LPTSTR lpCmdLine;
	// 窗口如何显示
	static int nCmdShow;
	// 渲染设备
	static ID3D11Device* pDevice;
	// 设备上下文
	static ID3D11DeviceContext* pDeviceContext;
	// D3D缓存链
	static IDXGISwapChain* pSwapChain;
	// 渲染视图
	static ID3D11RenderTargetView* pFrameView;
	// 深度视图
	static ID3D11DepthStencilView* pDepthView;
	// 深度缓冲
	static ID3D11Texture2D* pDepthBuffer;
	// 屏幕参数
	static int screenWidth, screenHeight;
	// 默认视点
	static D3D11_VIEWPORT viewport;
};