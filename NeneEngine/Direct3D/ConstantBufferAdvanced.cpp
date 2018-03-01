/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "ConstantBufferAdvanced.h"

BYTE* ConstantBufferAdvanced::perFrameData;
BYTE* ConstantBufferAdvanced::perObjectData;
std::vector<void*> ConstantBufferAdvanced::perFrameDataPtrs, ConstantBufferAdvanced::perObjectDataPtrs;
std::vector<size_t> ConstantBufferAdvanced::perFrameDataSize, ConstantBufferAdvanced::perObjectDataSize;
std::vector<std::string> ConstantBufferAdvanced::perFrameDataName, ConstantBufferAdvanced::perObjectDataName;
UINT ConstantBufferAdvanced::totalPerFrameSize, ConstantBufferAdvanced::totalPerObjectSize;
ID3D11Buffer* ConstantBufferAdvanced::pPerFrameConstantBuffer;
ID3D11Buffer* ConstantBufferAdvanced::pPerObjectConstantBuffer;

bool ConstantBufferAdvanced::registerBuffer(ConstantBufferType bt, void * bufferPtr, size_t bufferSize, const char * bufferName) {
	if (bt == UPDATE_PER_FRAME) {
		perFrameDataPtrs.push_back(bufferPtr);
		perFrameDataSize.push_back(bufferSize);
		perFrameDataName.push_back(std::string(bufferName));
		//
		totalPerFrameSize += bufferSize;
	}
	if (bt == UPDATE_PER_OBJECT) {
		perObjectDataPtrs.push_back(bufferPtr);
		perObjectDataSize.push_back(bufferSize);
		perObjectDataName.push_back(std::string(bufferName));
		//
		totalPerObjectSize += bufferSize;
	}
	return false;
}

bool ConstantBufferAdvanced::init() {
	//
	clear();
	// 
	perFrameData = new BYTE[totalPerFrameSize];
	perObjectData = new BYTE[totalPerObjectSize];
	// 每帧刷新的ConstantBuffer描述符初始化
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = totalPerFrameSize;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// 创建每帧刷新的ConstantBuffer
	HRESULT hr1 = Utils::getDevice()->CreateBuffer(&constantBufferDesc, nullptr, &pPerFrameConstantBuffer);
	checkResult(hr1, "Fail to create constant buffer per-frame!");
	// 每个物体变化的ConstantBuffer描述符
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = totalPerObjectSize;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// 创建每个物体变化的ConstantBuffer
	HRESULT hr2 = Utils::getDevice()->CreateBuffer(&constantBufferDesc, nullptr, &pPerObjectConstantBuffer);
	checkResult(hr2, "Fail to create constant buffer per-object!");
	return (SUCCEEDED(hr1) && SUCCEEDED(hr2));
}

void ConstantBufferAdvanced::clear() {
	if (perFrameData) delete[] perFrameData;
	if (perObjectData) delete[] perObjectData;
	if (pPerFrameConstantBuffer) pPerFrameConstantBuffer->Release();
	if (pPerObjectConstantBuffer) pPerObjectConstantBuffer->Release();
}
