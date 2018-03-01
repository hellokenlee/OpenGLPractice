/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <stdio.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include "Utils.h"
#include "Types.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

//
//	着色器类
//

class Shader {
public:
	// 构造
	Shader(const WCHAR* vsFilePath, const WCHAR* fsFilePath, VertexFormat lf);
	Shader(const CHAR* vsSource, const SIZE_T vsSourceSize, const CHAR* psSource, const SIZE_T psSourceSize, VertexFormat vf);
	// 析构
	~Shader ();
	//
	bool addOptionalShader(const WCHAR* filePath, ShaderType st, 
		std::vector<D3D11_SO_DECLARATION_ENTRY> pDecl = std::vector<D3D11_SO_DECLARATION_ENTRY>(0), UINT perVertexSize = 0);
	// 使用
	void use();
	// 根据顶点格式生成Layout
	void createLayout(VertexFormat lf);
	void createLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> &lfDesc);
	// 获取着色器的布局样式
	inline VertexFormat getLayoutFormat() {
		return layoutFormat;
	}
private:
	// 着色器编译后指针
	ID3DBlob *pVSBlob;
	ID3DBlob *pPSBlob;
	ID3DBlob *pGSBlob;
	// 着色器指针
	ID3D11VertexShader* pVS;
	ID3D11PixelShader* pPS;
	ID3D11GeometryShader *pGS;
	// 指向Layout的指针
	ID3D11InputLayout *pLayout;
	// 顶点格式
	VertexFormat layoutFormat;
private:
	// 禁止拷贝构造
	Shader(const Shader& o);
	// 从文件编译着色器
	bool compileShaderFile(const WCHAR* shaderFilePath, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob **pShader);
	//
	bool compileShaderSource(const CHAR* shaderSrc, SIZE_T sourceSize, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob **ppShaderBlob);
};

