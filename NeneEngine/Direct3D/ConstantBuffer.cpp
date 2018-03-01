/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "ConstantBuffer.h"

using namespace DirectX;

PerFrameConstantBuffer ConstantBuffer::perFrameCBData;
PerObjectConstantBuffer ConstantBuffer::perObjectCBData;
ID3D11Buffer* ConstantBuffer::pPerFrameConstantBuffer = nullptr;
ID3D11Buffer* ConstantBuffer::pPerObjectConstantBuffer = nullptr;

CBSpotLight::CBSpotLight(Vec3 _position, Vec4 _ambient, Vec4 _diffuse,
	Vec4 _specular, FLOAT innerCF, FLOAT outterCF, FLOAT atten0, 
	FLOAT atten1, FLOAT atten2) : 
	ambient(_ambient), diffuse(_diffuse), specular(_specular), 
	attenuation0(atten0), attenuation1(atten1), attenuation2(atten2),
	position(_position), innerCutoff(innerCF), outterCutoff(outterCF) 
	{}

CBDirectionalLight::CBDirectionalLight(Vec3 _direction, Vec4 _ambient,
	Vec4 _diffuse, Vec4 _specular) : 
	ambient(_ambient), diffuse(_diffuse), specular(_specular), 
	direction(_direction) {}

CBPointLight::CBPointLight(Vec3 _position, Vec4 _ambient, Vec4 _diffuse,
	Vec4 _specular, FLOAT atten0, FLOAT atten1, FLOAT atten2) :
	ambient(_ambient), diffuse(_diffuse), specular(_specular),
	position(_position), attenuation0(atten0), attenuation1(atten1), 
	attenuation2(atten2) {}

bool ConstantBuffer::init() {
	// 每帧刷新的ConstantBuffer描述符初始化
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(PerFrameConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// 创建每帧刷新的ConstantBuffer
	HRESULT hr1 = Utils::getDevice()->CreateBuffer(&constantBufferDesc, nullptr, &pPerFrameConstantBuffer);
	checkResult(hr1, "Fail to create constant buffer per-frame!");
	// 每个物体变化的ConstantBuffer描述符
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(PerObjectConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// 创建每个物体变化的ConstantBuffer
	HRESULT hr2 = Utils::getDevice()->CreateBuffer(&constantBufferDesc, nullptr, &pPerObjectConstantBuffer);
	checkResult(hr2, "Fail to create constant buffer per-object!");
	// 输出
	dLog("[Info] Constant Buffer Usage:\n    Per Frame Constant Buffer Size : %d;\n    Per Object Constant Buffer Size: %d; \n\n",
		sizeof(perFrameCBData), sizeof(perObjectCBData));
	return (SUCCEEDED(hr1) && SUCCEEDED(hr2));
}

void ConstantBuffer::clear() {
	if (pPerFrameConstantBuffer) pPerFrameConstantBuffer->Release();
	if (pPerObjectConstantBuffer) pPerObjectConstantBuffer->Release();
}