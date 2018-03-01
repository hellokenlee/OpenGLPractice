/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "ShadowMap.h"

#define lightVolumeWidth 40.0f

using namespace DirectX::SimpleMath;

ShadowMap::ShadowMap(UINT _width, UINT _height) : width(_width), height(_height), 
	pDepthMapDSV(nullptr), pDepthMapSRV(nullptr), pRasterState(nullptr) {
	// 初始化空的渲染目标
	ppNullRTV[0] = nullptr;
	// 初始化光源的视点
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = (FLOAT)width;
	viewPort.Height = (FLOAT)height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	// 设置要绘制的Texture描述符
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	// 创建纹理
	ID3D11Texture2D* pDepthMap = nullptr;
	HRESULT hr1 = Utils::getDevice()->CreateTexture2D(&texDesc, nullptr, &pDepthMap);
	checkResult(hr1, "Unable to create shadowmap(%d x %d)!", width, height);
	// 使用纹理创建DSV
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Flags = 0;
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;
	HRESULT hr2 = Utils::getDevice()->CreateDepthStencilView(pDepthMap, &DSVDesc, &pDepthMapDSV);
	checkResult(hr2, "Unable to create shadowmap's DSV!");
	// 使用纹理创建SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	HRESULT hr3 = Utils::getDevice()->CreateShaderResourceView(pDepthMap, &SRVDesc, &pDepthMapSRV);
	checkResult(hr3, "Unable to create shadowmap's SRV");
	// 初始化采样器描述符
	D3D11_SAMPLER_DESC sampler2DDesc;
	ZeroMemory(&sampler2DDesc, sizeof(sampler2DDesc));
	//sampler2DDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT; // for PS:　texShadowMap.SampleCmpLevelZero(...) 
	//sampler2DDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL; // for PS:　texShadowMap.SampleCmpLevelZero(...) 
	sampler2DDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler2DDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler2DDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler2DDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler2DDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler2DDesc.BorderColor[0] = 1.0f;
	sampler2DDesc.BorderColor[1] = 1.0f;
	sampler2DDesc.BorderColor[2] = 1.0f;
	sampler2DDesc.BorderColor[3] = 1.0f;
	sampler2DDesc.MinLOD = 0;
	sampler2DDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// 创建采样器
	HRESULT hr4 = Utils::getDevice()->CreateSamplerState(&sampler2DDesc, &pSamplerState);
	checkResult(hr4, "Failed to create sampler.");
	// 初始化
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	//rasterDesc.DepthBias = 500;
	//rasterDesc.SlopeScaledDepthBias = 0.01f;
	//!TODO: 使用 INT DepthBias; FLOAT DepthBiasClamp; FLOAT SlopeScaledDepthBias; 处理绘制球体时产生的acning问题
	HRESULT hr5 = Utils::getDevice()->CreateRasterizerState(&rasterDesc, &pRasterState);
	checkResult(hr5, "Fail to create Rasterizer state.");
	// 释放纹理
	if (pDepthMap) pDepthMap->Release();
}

ShadowMap::~ShadowMap() {
	if (pDepthMapSRV) pDepthMapSRV->Release();
	if (pDepthMapDSV) pDepthMapDSV->Release();
	if (pRasterState) pRasterState->Release();
}

void ShadowMap::setLightDirection(Vec3 dir) {
	lightView = Matrix::CreateLookAt(dir, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
	lightProjection = Matrix::CreateOrthographicOffCenter(-lightVolumeWidth, lightVolumeWidth, -lightVolumeWidth, lightVolumeWidth, 1.0f, 100.5f);
	lightSpaceMatrix = lightView * lightProjection;
}

void ShadowMap::begin() {
	// 存储之前的渲染状态
	numViewPorts = 1;
	Utils::getContext()->RSGetViewports(&numViewPorts, &pPrevViewPort);
	Utils::getContext()->OMGetRenderTargets(1, &pPrevRTV, &pPrevDSV);
	Utils::getContext()->RSGetState(&pPrevRS);
	// 设置视点
	Utils::getContext()->RSSetViewports(1, &viewPort);
	// 设置空的渲染对象
	Utils::getContext()->OMSetRenderTargets(1, ppNullRTV, pDepthMapDSV);
	// 清空深度缓冲
	Utils::getContext()->ClearDepthStencilView(pDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// 写入CB
	ConstantBuffer::perFrameCBData.lightSpaceMatrix = lightSpaceMatrix.Transpose();
	//
	Utils::getContext()->RSSetState(pRasterState);
}

void ShadowMap::end() {
	// 恢复之前的渲染状态
	Utils::getContext()->RSSetViewports(numViewPorts, &pPrevViewPort);
	Utils::getContext()->OMSetRenderTargets(1, &pPrevRTV, pPrevDSV);
	Utils::getContext()->RSSetState(pPrevRS);
}

void ShadowMap::use(const UINT slot) {
	// 设置采样器和纹理
	Utils::getContext()->PSSetShaderResources(slot, 1, &pDepthMapSRV);
	Utils::getContext()->PSSetSamplers(slot, 1, &pSamplerState);
}
