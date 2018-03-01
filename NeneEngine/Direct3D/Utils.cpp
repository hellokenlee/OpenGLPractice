/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Utils.h"

// 静态成员定义
const char Utils::defaultClassName[] = "NeneWindowClass";
HWND Utils::hWnd = nullptr;
HINSTANCE Utils::hInstance = nullptr;
HINSTANCE Utils::hPrevInstance = nullptr;
LPTSTR  Utils::lpCmdLine = nullptr;
int Utils::nCmdShow = -1;
IDXGISwapChain* Utils::pSwapChain = nullptr;
ID3D11Device* Utils::pDevice = nullptr;
ID3D11DeviceContext* Utils::pDeviceContext = nullptr;
bool Utils::isWindowShouldClose = false;
MSG Utils::msg;
ID3D11RenderTargetView* Utils::pFrameView = nullptr;
ID3D11DepthStencilView* Utils::pDepthView = nullptr;
ID3D11Texture2D* Utils::pDepthBuffer = nullptr;
ID3D11RasterizerState* Utils::polygonModes[3];
PolygonMode Utils::nowPolygonMode = MODE_POLYGON_CULL;
int Utils::screenWidth = 0, Utils::screenHeight = 0;
D3D11_VIEWPORT Utils::viewport = {};

// 静态成员方法
// ...
void Utils::init(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPTSTR _lpCmdLine, int _nCmdShow) {
	hInstance = _hInstance;
	hPrevInstance = _hPrevInstance;
	lpCmdLine = _lpCmdLine;
	nCmdShow = _nCmdShow;
}

HWND Utils::createWindow(const char* windowName, int width, int height) {
	// 初始化检查
	if (hInstance == nullptr) {
		printf("[Error] Please init first.\n");
		return nullptr;
	}
	// Window 类 初始化
	WNDCLASSEX winClass = {};
	ZeroMemory(&winClass, sizeof(WNDCLASSEX));
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = defaultWindowProc;
	winClass.hInstance = hInstance;
	winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//winClass.hbrBackground = (HBRUSH)COLOR_WINDOW; // 兼容 alt+enter 全屏
	winClass.lpszClassName = defaultClassName;
	// 向系统注册该类
	RegisterClassEx(&winClass);
	// 获取屏幕分辨率
	RECT screen;
	const HWND desktop = GetDesktopWindow();
	GetWindowRect(desktop, &screen);
	// 计算窗口大小
	screenWidth = width;
	screenHeight = height;
	RECT winRect = { 0, 0, width, height };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	const int winWidth = winRect.right - winRect.left;
	const int winHeight = winRect.bottom - winRect.top;
	// 计算正中间位置
	int wndStartPosX = ((screen.right - screen.left) - winWidth) / 2;
	int wndStartPosY = ((screen.bottom - screen.top) - winHeight) / 2;
	// 创建窗口
	Utils::hWnd = CreateWindowEx(0, defaultClassName, windowName,
		WS_OVERLAPPEDWINDOW, wndStartPosX, wndStartPosY, winWidth, winHeight,
		nullptr, nullptr, hInstance, nullptr);
	// 终端输出错误信息
	if (Utils::hWnd == nullptr) {
		printf("[Error] Failed to create window.\n");
	}
	// 弹框错误信息
	if (Utils::hWnd == nullptr) {
		int errorCode = GetLastError();
		char errorBuff[256];
		sprintf(errorBuff, "Can not create window.\nError Code: %d\n", errorCode);
		MessageBox(nullptr, errorBuff, "Error", MB_ICONEXCLAMATION | MB_OK);
		exit(-1);
	}
	// 显示窗口
	ShowWindow(Utils::hWnd, Utils::nCmdShow);
	// 初始化窗口状态
	isWindowShouldClose = false;
	// 返回
	return Utils::hWnd;
}

void Utils::pollEvents() {
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		// 如果有消息，处理消息
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) {
			isWindowShouldClose = true;
		}
	}
}

LRESULT Utils::defaultWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Utils::initDirectX() {
	// 防止内存泄露
	cleanDirectX();
	// 初始化渲染设备和缓冲链
	if (!initDeviceAndSwapChain()) exit(-1);
	// 初始化渲染视图
	if (!initRenderTarget()) exit(-1);
	// 初始化深度/模板缓冲
	if (!initDepthBuffer()) exit(-1);
	// 初始化常用的渲染模式
	D3D11_RASTERIZER_DESC polygonModeDesc;
	ZeroMemory(&polygonModeDesc, sizeof(polygonModeDesc));
	// 绘制面模式
	polygonModeDesc.FillMode = D3D11_FILL_SOLID;
	polygonModeDesc.CullMode = D3D11_CULL_BACK;
	HRESULT hr = pDevice->CreateRasterizerState(&polygonModeDesc, &polygonModes[MODE_POLYGON_CULL]);
	// 线框模式
	polygonModeDesc.FillMode = D3D11_FILL_WIREFRAME;
	polygonModeDesc.CullMode = D3D11_CULL_NONE;
	hr = pDevice->CreateRasterizerState(&polygonModeDesc, &polygonModes[MODE_WIREFRAME]);
	// 设置渲染目标 (1, 渲染视图, 深度视图)
	pDeviceContext->OMSetRenderTargets(1, &pFrameView, pDepthView);
	pDeviceContext->RSSetState(polygonModes[MODE_POLYGON_CULL]);
	//
	// 初始化视点
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = float(screenWidth);
	viewport.Height = float(screenHeight);
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	// 设置视点
	pDeviceContext->RSSetViewports(1, &viewport);
}

bool Utils::initDeviceAndSwapChain() {
	// 初始化缓冲描述符(用于初始交换链)
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Width = screenWidth;
	bufferDesc.Height = screenHeight;
	bufferDesc.RefreshRate.Numerator = 60;                                 // 刷新率分母
	bufferDesc.RefreshRate.Denominator = 1;                                // 刷新率分子  
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                        // 缓冲格式RGBA共32bits(0-255)
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;    // 扫描线方向(指定从哪个方向开始填充缓冲? )
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                    // 指定窗口被拉伸后怎么处理图片
	// 初始化一个缓冲交换链描述符
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;                           // 指定上面定义的缓冲
	swapChainDesc.BufferCount = 1;                                   // 只有一个 后端缓冲
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // 声明这个交换链用于输出
	swapChainDesc.OutputWindow = hWnd;                               // 绑定输出的窗口句柄
	swapChainDesc.SampleDesc.Count = 4;                              // MSAA采样数
	swapChainDesc.SampleDesc.Quality = 0;                            // 采样质量
	swapChainDesc.Windowed = true;                                   // 窗口显示(而非全屏)
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // 允许通过 alt+enter 切换全屏
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;             // 让驱动决定置换缓冲后如何处理旧的缓冲
	// 创建Device和SwapChain !FIXME: 这里处理不了多显示器
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, nullptr, &pDeviceContext
	);
	checkResult(hr, "[Error] Failed to init render pDeviceice and swap chain! \n");
	return SUCCEEDED(hr);
}

bool Utils::initDepthBuffer() {
	// 初始化深度缓冲描述符
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 4;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// 创建深度缓冲
	HRESULT hr1 = pDevice->CreateTexture2D(&depthBufferDesc, nullptr, &pDepthBuffer);
	checkResult(hr1, "Failed to create depth buffer!");
	// 创建深度视图
	HRESULT hr2 = pDevice->CreateDepthStencilView(pDepthBuffer, nullptr, &pDepthView);
	checkResult(hr2, "Failed to create depth view!");
	return (SUCCEEDED(hr1) && SUCCEEDED(hr2));
}


bool Utils::initRenderTarget() {
	// 渲染缓冲
	ID3D11Texture2D* pFrameBuffer = nullptr;
	// 获取后缓冲的地址
	HRESULT hr1 = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pFrameBuffer);
	checkResult(hr1, "Failed to get frame buffer!");
	// 在后缓冲中创建渲染目标
	HRESULT hr2 = pDevice->CreateRenderTargetView(pFrameBuffer, nullptr, &pFrameView);
	checkResult(hr2, "Failed to create render target view!");
	// 清理
	pFrameBuffer->Release();
	return (SUCCEEDED(hr1) && SUCCEEDED(hr2));
}

void Utils::setPolygonMode(PolygonMode mode) {
	nowPolygonMode = mode;
	pDeviceContext->RSSetState(polygonModes[mode]);
}

void Utils::_checkError(const char* filename, const char* func, const int line) {
	int errorCode = GetLastError();
	if (errorCode != 0) {
		std::string filenameStr(filename);
		int lastSplashPos = filenameStr.rfind('\\');
		filenameStr = filenameStr.substr(lastSplashPos + 1, std::string::npos);
		printf("[Error]: \n");
		printf("    File: %s \n", filenameStr.c_str());
		printf("    Func: %s(...) \n", func);
		printf("    Line: %d \n", line);
		printf("    Error Code: %d\n\n", errorCode);
	}
}

void Utils::_checkResult(const char * filename, const char * func, const int line, HRESULT hr, const char* errorStr, ...) {
	if (hr != S_OK) {
		std::string filenameStr(filename);
		int lastSplashPos = filenameStr.rfind('\\');
		filenameStr = filenameStr.substr(lastSplashPos + 1, std::string::npos);
		//
		va_list argptr;
		va_start(argptr, errorStr);
		va_end(argptr);
		//
		printf("[Error] ");
		vprintf(errorStr, argptr);
		printf(" In: %s, %s(...), %d th line.\n\n", filenameStr.c_str(), func, line);
	}
}

void Utils::_checkFileExist(const WCHAR* filename) {
	if(_waccess(filename, 0) == -1){
		printf("[Error] File not exist! (%ws)\n\n", filename);
	}
}

void Utils::cleanDirectX() {
	if (pSwapChain) {
		pSwapChain->SetFullscreenState(FALSE, nullptr);
		pSwapChain->Release();
	}
	if(pDevice) pDevice->Release();
	if(pDeviceContext) pDeviceContext->Release();
	if(pFrameView) pFrameView->Release();
	if(pDepthBuffer) pDepthBuffer->Release();
	if(pDepthView) pDepthView->Release();
	for (int i = 0; i < POLYGON_MODES_NUM; ++i) {
		if (polygonModes[i]) polygonModes[i]->Release();
	}

}

void Utils::clearColor(FLOAT r, FLOAT g, FLOAT b) {
	static float colorRGBA[4] = { r, g, b, 1.0f };
	pDeviceContext->ClearRenderTargetView(pFrameView, colorRGBA);
}

void Utils::clear() {
	pDeviceContext->ClearDepthStencilView(pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Utils::swapBuffer() {
	pSwapChain->Present(1, 0);
}
