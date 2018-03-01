/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "Utils.h"
#include "TextureLoader\WICTextureLoader.h"
#include "TextureLoader\DDSTextureLoader.h"

class TextureCube {
public:
	TextureCube(const WCHAR* filePath[6]);
	TextureCube(const WCHAR* ddsFilePath);
	~TextureCube();
	void use();
private:
	TextureCube();
	TextureCube(const TextureCube& other);
	ID3D11Texture2D* pTextureCube;
	ID3D11ShaderResourceView* pTextureCubeView;
	ID3D11SamplerState* pTextureCubeState;
};