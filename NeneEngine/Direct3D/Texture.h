/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <d3d11.h>
#include <string.h>
#include <memory>
#include <vector>
#include "Utils.h"
#include "TextureLoader\WICTextureLoader.h"
#include "TextureLoader\DDSTextureLoader.h"

//
//		纹理类
//

//!TODO: 是否真的需要这么多Sampler
class Texture {
public:
	Texture(const WCHAR* filePath);
	~Texture();
	void use(const UINT slot = 0);
	static std::unique_ptr<Texture> createRandomTexture1D(const UINT size = 1024);
public:
	ID3D11ShaderResourceView * pSRV;
	ID3D11SamplerState* pSamplerState;
private:
	Texture();
	Texture(const Texture& other);
};