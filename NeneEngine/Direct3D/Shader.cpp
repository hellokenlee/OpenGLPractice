/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Shader.h"

Shader::Shader(const WCHAR* pVSFilePath, const WCHAR* pPSFilePath, VertexFormat lf) {
	//
	checkFileExist(pVSFilePath);
	checkFileExist(pPSFilePath);
	// 初始化
	pVSBlob = nullptr;
	pPSBlob = nullptr;
	pGSBlob = nullptr;
	pVS = nullptr;
	pPS = nullptr;
	pGS = nullptr;
	pLayout = nullptr;
	layoutFormat = NO_FORMAT;
	// 编译着色器
	if (compileShaderFile(pVSFilePath, "VS", "vs_5_0", &pVSBlob)) {
		HRESULT hr0 = Utils::getDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVS);
		checkResult(hr0, "Failed to create vertex shader.");
	}
	if(compileShaderFile(pPSFilePath, "PS", "ps_5_0", &pPSBlob)) {
		HRESULT hr0 = Utils::getDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPS);
		checkResult(hr0, "Failed to create pixel shader.");
	}
	// 创建Layout
	if(pVSBlob) createLayout(lf);
}

Shader::Shader(const CHAR* vsSource, const SIZE_T vsSourceSize, const CHAR* psSource, const SIZE_T psSourceSize, VertexFormat lf) {
	// 初始化
	pVSBlob = nullptr;
	pPSBlob = nullptr;
	pGSBlob = nullptr;
	pVS = nullptr;
	pPS = nullptr;
	pGS = nullptr;
	pLayout = nullptr;
	layoutFormat = NO_FORMAT;
	// 编译&&创建着色器
	if (compileShaderSource(vsSource, vsSourceSize, "VS", "vs_5_0", &pVSBlob)) {
		HRESULT hr0 = Utils::getDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVS);
		checkResult(hr0, "Failed to create vertex shader.");
	}
	if (compileShaderSource(psSource, psSourceSize, "PS", "ps_5_0", &pPSBlob)) {
		HRESULT hr0 = Utils::getDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPS);
		checkResult(hr0, "Failed to create pixel shader.");
	}
	// 创建Layout
	if (pVSBlob) createLayout(lf);
}

Shader::~Shader() {
	if(pVSBlob) pVSBlob->Release();
	if(pPSBlob) pPSBlob->Release();
	if(pGSBlob) pGSBlob->Release();
	if(pVS) pVS->Release();
	if(pPS) pPS->Release();
	if(pGS) pGS->Release();
	if(pLayout) pLayout->Release();
}

bool Shader::addOptionalShader(const WCHAR * filePath, ShaderType st, 
	std::vector<D3D11_SO_DECLARATION_ENTRY> pDecl, UINT perVertexSize) {
	switch (st) {
		case GEOMETRY_SHADER: {
			if (compileShaderFile(filePath, "GS", "gs_5_0", &pGSBlob)) {
				HRESULT hr = Utils::getDevice()->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), nullptr, &pGS);
				checkResult(hr, "Failed to create geometry shader.");
			}
			break;
		}
		case GEOMETRY_SHADER_STREAM_OUT: {
			if (compileShaderFile(filePath, "GS", "gs_5_0", &pGSBlob)) {
				UINT strideArray[1] = { perVertexSize };
				HRESULT hr = Utils::getDevice()->CreateGeometryShaderWithStreamOutput(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(),
					pDecl.data(), pDecl.size(), strideArray, 1, D3D11_SO_NO_RASTERIZED_STREAM, nullptr, &pGS);
				checkResult(hr, "Failed to create geometry shader with stream-out.");
			}
			break;
		}
		default: {
			dLog("[Error] Unsupport optional shader type!");
		}
	}
	return false;
}

bool Shader::compileShaderFile(const WCHAR* shaderFilePath, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob **ppShaderBlob) {
	// 错误信息
	ID3DBlob* pErrorBlob = nullptr;
	// 编译着色器
	HRESULT hr = D3DCompileFromFile(shaderFilePath, nullptr, nullptr, entryPoint, shaderModel,
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, ppShaderBlob, &pErrorBlob);
	// 输出错误信息
	if (FAILED(hr)) {
		if (pErrorBlob) {
			wprintf(L"[Error] Failed to Compile %hs:\n", entryPoint);
			printf("%s\n", (const char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return false;
	}
	// 清理
	if (pErrorBlob) pErrorBlob->Release();
	//
	return true;
}

bool Shader::compileShaderSource(const CHAR* shaderSrc, SIZE_T sourceSize, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob **ppShaderBlob) {
	// 错误信息
	ID3DBlob* pErrorBlob = nullptr;
	// 编译着色器
	HRESULT hr = D3DCompile(shaderSrc, sourceSize, nullptr, nullptr, nullptr, entryPoint,
		shaderModel, D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, ppShaderBlob, &pErrorBlob);
	// 输出错误信息
	if (FAILED(hr)) {
		if (pErrorBlob) {
			wprintf(L"[Error] Failed to Compile %hs:\n", entryPoint);
			printf("%s\n", (const char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return false;
	}
	// 清理
	if (pErrorBlob) pErrorBlob->Release();
	//
	return true;
}

void Shader::use() {
	// 设置顶点着色器
	Utils::getContext()->VSSetShader(pVS, nullptr, 0);
	// 设置像素着色器
	Utils::getContext()->PSSetShader(pPS, nullptr, 0);
	// 设置几何着色器
	if (pGS) 
		Utils::getContext()->GSSetShader(pGS, nullptr, 0);
	else
		Utils::getContext()->GSSetShader(nullptr, nullptr, 0);
	// 设置输入Layout
	Utils::getContext()->IASetInputLayout(pLayout);
}


void Shader::createLayout(VertexFormat lf) {
	// 删除之前的Layout
	layoutFormat = lf;
	if (pLayout) pLayout->Release();
	// 每个顶点的属性数量
	UINT perVertextAttribNum = 0;
	// 按照顶点格式指定Layout格式
	D3D11_INPUT_ELEMENT_DESC *layoutDesc = nullptr;
	switch (lf) {
		case POSITIONS: {
			perVertextAttribNum = 1;
			layoutDesc = new D3D11_INPUT_ELEMENT_DESC[perVertextAttribNum];
			layoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			break;
		}
		case POSITIONS_COLORS: {
			perVertextAttribNum = 2;
			layoutDesc = new D3D11_INPUT_ELEMENT_DESC[perVertextAttribNum];
			layoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layoutDesc[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			break;
		}
		case POSITIONS_TEXTURES: {
			perVertextAttribNum = 2;
			layoutDesc = new D3D11_INPUT_ELEMENT_DESC[perVertextAttribNum];
			layoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layoutDesc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			break;
		}
		case POSITIONS_NORMALS_COLORS: {
			perVertextAttribNum = 3;
			layoutDesc = new D3D11_INPUT_ELEMENT_DESC[perVertextAttribNum];
			layoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layoutDesc[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layoutDesc[2] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			break;
		}
		case POSITIONS_NORMALS_TEXTURES: {
			perVertextAttribNum = 3;
			layoutDesc = new D3D11_INPUT_ELEMENT_DESC[perVertextAttribNum];
			layoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layoutDesc[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layoutDesc[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			break;
		}
		case POSITIONS_NORMALS_TEXTURES_IMODELS: {
			perVertextAttribNum = 7;
			layoutDesc = new D3D11_INPUT_ELEMENT_DESC[perVertextAttribNum];
			//
			layoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layoutDesc[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layoutDesc[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			//
			layoutDesc[3] = { "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
			layoutDesc[4] = { "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
			layoutDesc[5] = { "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
			layoutDesc[6] = { "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
			break;
		}
		case NO_FORMAT: {
			return;
		}
	}
	// 创建新Layout
	HRESULT hr = Utils::getDevice()->CreateInputLayout(layoutDesc, perVertextAttribNum, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pLayout);
	checkResult(hr, "Failded to create vertex input layout.");
	// 清理
	if (layoutDesc) delete[] layoutDesc;
}

void Shader::createLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> &lfDesc) {
	if (pVSBlob) {
		HRESULT hr = Utils::getDevice()->CreateInputLayout(lfDesc.data(), lfDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pLayout);
		checkResult(hr, "Failded to create vertex input layout.");
	} else {
		dLog("[Error] Failded to create vertex input layout: empty vertex shader.\n");
	}
	
}

