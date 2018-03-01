#include "FrameBufferTexture.h"
#include "Texture.h"
#include "Geometry.h"



FrameBufferTexture::FrameBufferTexture(UINT width, UINT height) {
	// 初始化
	pMSAATexture = nullptr;
	pTexture = nullptr;
	pSRV = nullptr;
	pRTV = nullptr;
	pSamplerState = nullptr;
	pPrevRTV = nullptr;
	pPrevDSV = nullptr;
	// 创建RTV纹理
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 4;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	HRESULT hr0 = Utils::getDevice()->CreateTexture2D(&texDesc, nullptr, &pMSAATexture);
	checkResult(hr0, "Failed to create frame buffer texture.");
	// 创建SRV纹理
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	HRESULT hr1 = Utils::getDevice()->CreateTexture2D(&texDesc, nullptr, &pTexture);
	checkResult(hr1, "Failed to create shader source texture.");
	// 创建RenderTarget -> 类似FrameBufferObject?
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(RTVDesc));
	RTVDesc.Format = texDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	RTVDesc.Texture2D.MipSlice = 0;
	HRESULT hr2 = Utils::getDevice()->CreateRenderTargetView(pMSAATexture, &RTVDesc, &pRTV);
	checkResult(hr2, "Failed to create frame buffer.");
	// 创建SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = texDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;
	HRESULT hr3 = Utils::getDevice()->CreateShaderResourceView(pTexture, &SRVDesc, &pSRV);
	checkResult(hr3, "Failed to create SRV.");
	// 创建采样器
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr4 = Utils::getDevice()->CreateSamplerState(&samplerDesc, &pSamplerState);
	checkResult(hr4, "Failed to create sampler.");

}

FrameBufferTexture::~FrameBufferTexture() {
	if(pMSAATexture) pMSAATexture->Release();
	if(pTexture) pTexture->Release();
	if(pSRV) pSRV->Release();
	if(pRTV) pRTV->Release();
	if(pSamplerState) pSamplerState->Release();
}

void FrameBufferTexture::begin(){
	// 保存原来的渲染对象
	Utils::getContext()->OMGetRenderTargets(1, &pPrevRTV, &pPrevDSV);
	// 设置渲染对象
	Utils::getContext()->OMSetRenderTargets(1, &pRTV, pPrevDSV);
	// 填充背景色
	static float colorRGBA[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	Utils::getContext()->ClearRenderTargetView(pRTV, colorRGBA);
	// 清空深度缓冲
	Utils::getContext()->ClearDepthStencilView(pPrevDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void FrameBufferTexture::end() {
	// 恢复原来的渲染对象
	Utils::getContext()->OMSetRenderTargets(1, &pPrevRTV, pPrevDSV);
	// 把MSAA纹理blit到普通纹理
	Utils::getContext()->ResolveSubresource(pTexture, 0, pMSAATexture, 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
}
void FrameBufferTexture::use(UINT slot) {
	Utils::getContext()->PSSetShaderResources(slot, 1, &pSRV);
	Utils::getContext()->PSSetSamplers(slot, 1, &pSamplerState);
}