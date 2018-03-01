/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "Utils.h"

class FrameBufferTexture {
public:
	FrameBufferTexture(UINT width, UINT height);
	~FrameBufferTexture();
	void begin();
	void end();
	void use(UINT slot);
public:
	FrameBufferTexture();
	FrameBufferTexture(const FrameBufferTexture& other);
	//
	ID3D11Texture2D *pMSAATexture;
	ID3D11Texture2D *pTexture;
	ID3D11ShaderResourceView *pSRV;
	ID3D11RenderTargetView *pRTV;
	ID3D11SamplerState* pSamplerState;
	//
	ID3D11RenderTargetView* pPrevRTV;
	ID3D11DepthStencilView* pPrevDSV;
	//
};