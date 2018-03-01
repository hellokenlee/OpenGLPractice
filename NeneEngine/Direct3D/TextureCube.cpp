/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "TextureCube.h"

TextureCube::TextureCube(const WCHAR * filePath[6]) {
	ID3D11Resource* pTextures[6];
	// 读取纹理图片;!NOTE: 每张图片的分辨率必须一致。
	for (UINT i = 0; i < 6; ++i) {
		checkFileExist(filePath[i]);
		pTextures[i] = nullptr;
		HRESULT hr = DirectX::CreateWICTextureFromFile(Utils::getDevice(), filePath[i], &pTextures[i], nullptr);
		checkResult(hr, "Failed to load cube texture file.");
#ifdef DEBUG
		D3D11_TEXTURE2D_DESC texDesc;
		((ID3D11Texture2D*)pTextures[0])->GetDesc(&texDesc);
		printf("[Info] Load texture with %d x %d.(%ls)\n\n", texDesc.Width, texDesc.Height, filePath[i]);
#endif
	}
	// 
	D3D11_TEXTURE2D_DESC texDesc;
	((ID3D11Texture2D*)pTextures[0])->GetDesc(&texDesc);
	// 整个Cube-Texture描述符
	D3D11_TEXTURE2D_DESC texCubeDesc;
	ZeroMemory(&texCubeDesc, sizeof(texCubeDesc));
	texCubeDesc.Width = texDesc.Width;
	texCubeDesc.Height = texDesc.Height;
	texCubeDesc.MipLevels = texDesc.MipLevels;
	texCubeDesc.Format = texDesc.Format;
	texCubeDesc.ArraySize = 6;
	texCubeDesc.SampleDesc.Count = 1;
	texCubeDesc.SampleDesc.Quality = 0;
	texCubeDesc.Usage = D3D11_USAGE_DEFAULT;
	texCubeDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texCubeDesc.CPUAccessFlags = 0;
	texCubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	//
	HRESULT hr1 = Utils::getDevice()->CreateTexture2D(&texCubeDesc, 0, &pTextureCube);
	checkResult(hr1, "Failed to create cube texture.");
	// 把6张纹理图片复制到Cube-Texture中
	D3D11_BOX sourceBox;
	for (UINT i = 0; i < 6; ++i) {
		for (UINT mipLevel = 0; mipLevel < texCubeDesc.MipLevels; ++mipLevel) {
			sourceBox.left = 0;
			sourceBox.right = (texCubeDesc.Width >> mipLevel);
			sourceBox.top = 0;
			sourceBox.bottom = (texCubeDesc.Height >> mipLevel);
			sourceBox.front = 0;
			sourceBox.back = 1;
			Utils::getContext()->CopySubresourceRegion(pTextureCube,
				D3D11CalcSubresource(mipLevel, i, texCubeDesc.MipLevels), 0, 0, 0, pTextures[i], mipLevel, &sourceBox);
		}
	}
	// 创建Textuer-View
	D3D11_SHADER_RESOURCE_VIEW_DESC texCubeViewDesc;
	texCubeViewDesc.Format = texCubeDesc.Format;
	texCubeViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	texCubeViewDesc.TextureCube.MostDetailedMip = 0;
	texCubeViewDesc.TextureCube.MipLevels = texCubeDesc.MipLevels;
	//
	HRESULT hr2 = Utils::getDevice()->CreateShaderResourceView(pTextureCube, &texCubeViewDesc, &pTextureCubeView);
	checkResult(hr2, "Failed to create cube texture view.");
	//
	// 初始化纹理格式描述符
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
	HRESULT hr3 = Utils::getDevice()->CreateSamplerState(&sampler2DDesc, &pTextureCubeState);
	checkResult(hr3, "Failed to create texture state.");
	// 清理
	for (UINT i = 0; i < 6; ++i) {
		if (pTextures[i]) pTextures[i]->Release();
	}
}

TextureCube::TextureCube(const WCHAR * ddsFilePath) {

}

TextureCube::~TextureCube() {
	if (pTextureCube) pTextureCube->Release();
	if (pTextureCubeView) pTextureCubeView->Release();
	if (pTextureCubeState) pTextureCubeState->Release();
}

void TextureCube::use() {
	Utils::getContext()->PSSetShaderResources(0, 1, &pTextureCubeView);
	Utils::getContext()->PSSetSamplers(0, 1, &pTextureCubeState);
}
