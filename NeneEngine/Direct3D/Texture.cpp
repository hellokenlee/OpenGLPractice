/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Texture.h"


Texture::Texture() : pSRV(nullptr), pSamplerState(nullptr) {}

Texture::Texture(const WCHAR* filePath) {
	//
	checkFileExist(filePath);
	// 初始化
	pSRV = nullptr;
	pSamplerState = nullptr;
	// 从图像文件中加载纹理
	HRESULT hr1 = S_OK;
	size_t len = wcslen(filePath);
	if (filePath[len - 4] == '.' && filePath[len - 3] == 'd' &&
		filePath[len - 2] == 'd' && filePath[len - 1] == 's') {
		hr1 = DirectX::CreateDDSTextureFromFile(Utils::getDevice(), filePath, nullptr, &pSRV);
		checkResult(hr1, "Failed to load DDS texture file.");
	} else {
		hr1 = DirectX::CreateWICTextureFromFile(Utils::getDevice(), filePath, nullptr, &pSRV);
		checkResult(hr1, "Failed to load image texture file.");
	}
	// 初始化纹理格式描述符
	if (SUCCEEDED(hr1)) {
		D3D11_SAMPLER_DESC sampler2DDesc;
		ZeroMemory(&sampler2DDesc, sizeof(sampler2DDesc));
		sampler2DDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler2DDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler2DDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler2DDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler2DDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler2DDesc.MinLOD = 0;
		sampler2DDesc.MaxLOD = D3D11_FLOAT32_MAX;
		// 创建采样器
		HRESULT hr2 = Utils::getDevice()->CreateSamplerState(&sampler2DDesc, &pSamplerState);
		checkResult(hr2, "Failed to create texture state.");
	}

}

Texture::~Texture() {
	if (pSRV) pSRV->Release();
	if (pSamplerState) pSamplerState->Release();
}

void Texture::use(const UINT slot) {
	Utils::getContext()->PSSetShaderResources(slot, 1, &pSRV);
	Utils::getContext()->PSSetSamplers(slot, 1, &pSamplerState);

	Utils::getContext()->VSSetShaderResources(slot, 1, &pSRV);
	Utils::getContext()->PSSetSamplers(slot, 1, &pSamplerState);

	Utils::getContext()->GSSetShaderResources(slot, 1, &pSRV);
	Utils::getContext()->GSSetSamplers(slot, 1, &pSamplerState);
}


std::unique_ptr<Texture> Texture::createRandomTexture1D(const UINT size) {
	std::vector<Vec4> randomVals(size);
	// 填充随机值
	for (UINT i = 0; i < size; ++i) {
		randomVals[i].x = randf(-1.0f, 1.0f);
		randomVals[i].y = randf(-1.0f, 1.0f);
		randomVals[i].z = randf(-1.0f, 1.0f);
		randomVals[i].w = randf(-1.0f, 1.0f);
	}
	
	// 初始化
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = randomVals.data();
	initData.SysMemPitch = size * sizeof(Vec4);
	initData.SysMemSlicePitch = 0;

	// 创建Tetxure
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = size;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	ID3D11Texture1D* randomTex = 0;
	HRESULT hr0 = Utils::getDevice()->CreateTexture1D(&texDesc, &initData, &randomTex);
	checkResult(hr0, "Failed to create random texture.");
	
	// 创建SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srvDesc.Texture1D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture1D.MostDetailedMip = 0;
	Texture *pTex = new Texture();
	HRESULT hr1 = Utils::getDevice()->CreateShaderResourceView(randomTex, &srvDesc, &(pTex->pSRV));
	checkResult(hr1, "Failed to create random SRV.");

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
	HRESULT hr3 = Utils::getDevice()->CreateSamplerState(&samplerDesc, &(pTex->pSamplerState));
	checkResult(hr3, "Failed to create random texture sampler.");
	//
	if (randomTex) randomTex->Release();
	// 
	return std::unique_ptr<Texture>(pTex);
}