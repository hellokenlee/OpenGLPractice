/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <vector>
#include "Utils.h"
#include "Types.h"

#define SLOT_PER_FRAME_CB 0
#define SLOT_PER_OBJECT_CB 1

//!IMPROVE: 分块Update可减少一次CPU拷贝

//
//	用于管理ConstantBuffer的静态类·改 —— 运行时确定CB结构
//

class ConstantBufferAdvanced{
public:
	// 
	static bool registerBuffer(ConstantBufferType bt, void* bufferPtr, size_t bufferSize, const char* bufferName = "NO-NAME");
	//
	static bool init();
	//
	static void clear();
	// 更新每帧需要更新的ConstantBuffer
	static inline void updatePerFrame() {
		// 拷贝数据(RAM->RAM)
		UINT startPos = 0;
		for (UINT i = 0; i < perFrameDataPtrs.size(); ++i) {
			memcpy((perFrameData + startPos), perFrameDataPtrs[i], perFrameDataSize[i]);
			startPos += perFrameDataSize[i];
		}
		// 更新(RAM->GRAM)
		Utils::getContext()->UpdateSubresource(pPerFrameConstantBuffer, 0, nullptr, perFrameData, 0, 0);
		Utils::getContext()->VSSetConstantBuffers(SLOT_PER_FRAME_CB, 1, &pPerFrameConstantBuffer);
	}
	// 更新每个物体需要更新的ConstantBuffer
	static inline void updatePerObject() {
		// 拷贝数据(RAM->RAM)
		UINT startPos = 0;
		for (UINT i = 0; i < perObjectDataPtrs.size(); ++i) {
			memcpy((perObjectData + startPos), perObjectDataPtrs[i], perObjectDataSize[i]);
			startPos += perObjectDataSize[i];
		}
		// 更新(RAM->GRAM)
		Utils::getContext()->UpdateSubresource(pPerObjectConstantBuffer, 0, nullptr, perObjectData, 0, 0);
		Utils::getContext()->VSSetConstantBuffers(SLOT_PER_OBJECT_CB, 1, &pPerObjectConstantBuffer);
	}
	// 
	static void showBufferComposition();
public:
	// 指向每帧更新的CB数据的指针
	static BYTE* perFrameData;
	// 指向每物体更新的CB数据的指针
	static BYTE* perObjectData;
	// 指向数据来源的指针
	static std::vector<void*> perFrameDataPtrs, perObjectDataPtrs;
	// 数据来源的大小
	static std::vector<size_t> perFrameDataSize, perObjectDataSize;
	// (可选)名字
	static std::vector<std::string> perFrameDataName, perObjectDataName;
	// CB的总大小
	static UINT totalPerFrameSize, totalPerObjectSize;
private:
	// 每帧需要更新的CB指针
	static ID3D11Buffer* pPerFrameConstantBuffer;
	// 每个物体需要更新的CB指针
	static ID3D11Buffer* pPerObjectConstantBuffer;
};