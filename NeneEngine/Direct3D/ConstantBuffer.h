/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "Utils.h"
#include "Types.h"

#define SLOT_PER_FRAME_CB 0
#define SLOT_PER_OBJECT_CB 1

#define DIRECTIONAL_LIGHT_NUM 1
#define POINT_LIGHT_NUM 2
#define SPOT_LIGHT_NUM 1

// 定向光源结构体  size: 4 * 3 + 3 + 1 =16
struct CBDirectionalLight {
	CBDirectionalLight() : CBDirectionalLight(Vec3(0.0f, 1.0f, 0.0f)) {}
	CBDirectionalLight(Vec3 _direction, Vec4 _ambient = Vec4(0.01f),
		Vec4 _diffuse = Vec4(0.5f), Vec4 _specular = Vec4(0.2f));
	Vec4 ambient;
	Vec4 diffuse;
	Vec4 specular;
	Vec3 direction;
	FLOAT padding;
};


// 点光源结构体  size: 4 * 3 + 3 + 5 = 20
struct CBPointLight {
	CBPointLight() : CBPointLight(Vec3(0.0f, 0.0f, 0.0f)){}
	CBPointLight(Vec3 _position, Vec4 _ambient = Vec4(0.05f), 
		Vec4 _diffuse = Vec4(0.5f), Vec4 _specular = Vec4(0.2f), 
		FLOAT atten0 = 1.0f, FLOAT atten1 = 0.2f, FLOAT atten2 = 0.9f);
	Vec4 ambient;
	Vec4 diffuse;
	Vec4 specular;
	Vec3 position;
	FLOAT attenuation0, attenuation1, attenuation2, padding0, padding1;
};

// 聚光灯结构体  size: 4 * 3 + 3 + 5 = 20
struct CBSpotLight {
	CBSpotLight() : CBSpotLight(Vec3(0.0f, 0.0f, 0.0f)){}
	CBSpotLight(Vec3 _position, Vec4 _ambient = Vec4(0.05f), Vec4 _diffuse = Vec4(0.5f),
		Vec4 _specular = Vec4(0.6f), FLOAT innerCF = 0.0f, FLOAT outterCF = 0.0f,
		FLOAT atten0 = 1.0f, FLOAT atten1 = 0.19f, FLOAT atten2 = 0.32f);
	Vec4 ambient;
	Vec4 diffuse;
	Vec4 specular;
	Vec3 position;
	FLOAT innerCutoff, outterCutoff, attenuation0, attenuation1, attenuation2;
};

// 每帧需要更新的常量缓冲(每帧绘制手动调用)
struct PerFrameConstantBuffer {
	PerFrameConstantBuffer() {}
	//
	Mat4 view;
	Mat4 projection;
	Vec3 cameraPosition; FLOAT padding;
	//
	CBDirectionalLight dirLights[DIRECTIONAL_LIGHT_NUM];
	CBPointLight pointLights[POINT_LIGHT_NUM];
	CBSpotLight spotLights[SPOT_LIGHT_NUM];
	//
	Mat4 lightSpaceMatrix;
	//
	Vec3 emitterPos; FLOAT dt;
	Vec3 emitterDir; FLOAT time;
};

// 每物体需要更新的常量缓冲(Shape->draw(...)时调用)
struct PerObjectConstantBuffer {
	PerObjectConstantBuffer() {}
	Mat4 model;
};

//
//	用于管理ConstantBuffer的静态类 - !NOTE: 编译时确定CB结构 -> 采用ConstantBufferAdvanced获得运行时确定CB结构功能
//

class ConstantBuffer {
public:
	// 初始化
	static bool init();
	// 清理
	static void clear();
	// 更新每帧需要更新的ConstantBuffer
	static inline void updatePerFrame() {
		Utils::getContext()->UpdateSubresource(pPerFrameConstantBuffer, 0, nullptr, &perFrameCBData, 0, 0);
		Utils::getContext()->VSSetConstantBuffers(SLOT_PER_FRAME_CB, 1, &pPerFrameConstantBuffer);
		Utils::getContext()->PSSetConstantBuffers(SLOT_PER_FRAME_CB, 1, &pPerFrameConstantBuffer);
	}
	// 更新每个物体需要更新的ConstantBuffer
	static inline void updatePerObject() {
		Utils::getContext()->UpdateSubresource(pPerObjectConstantBuffer, 0, nullptr, &perObjectCBData, 0, 0);
		Utils::getContext()->VSSetConstantBuffers(SLOT_PER_OBJECT_CB, 1, &pPerObjectConstantBuffer);
		Utils::getContext()->PSSetConstantBuffers(SLOT_PER_OBJECT_CB, 1, &pPerObjectConstantBuffer);
	}
public:
	static PerFrameConstantBuffer perFrameCBData;
	static PerObjectConstantBuffer perObjectCBData;
public:
	// 每帧需要更新的CB指针
	static ID3D11Buffer* pPerFrameConstantBuffer;
	// 每个物体需要更新的CB指针
	static ID3D11Buffer* pPerObjectConstantBuffer;
};
