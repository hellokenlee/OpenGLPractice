/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "Types.h"
#include "Utils.h"
#include "ConstantBuffer.h"

class ShadowMap {
public:
	// 创建ShadowMap
	ShadowMap(UINT _width, UINT _height);
	// 析构函数
	~ShadowMap();
	// 设置光源位置
	void setLightDirection(Vec3 pos);
	// 开始ShadowMap绘制
	void begin();
	// 结束ShadowMap绘制
	void end();
	// 使用ShadowMap
	void use(const UINT slot);
private:
	// 禁止默认构造和拷贝构造
	ShadowMap();
	ShadowMap(const ShadowMap & other);
	
	// ShadowMap 大小
	UINT width;
	UINT height;
	
	// ShadowMap的贴图
	ID3D11ShaderResourceView* pDepthMapSRV;
	// ShadowMap的深度缓存
	ID3D11DepthStencilView* pDepthMapDSV;
	// 空的渲染Buffer
	ID3D11RenderTargetView* ppNullRTV[1];
	// 视点
	D3D11_VIEWPORT viewPort;
	// 只Cull前面的光栅化状态
	ID3D11RasterizerState *pRasterState;

	// 用于保存上次的渲染上下文
	UINT numViewPorts;
	D3D11_VIEWPORT pPrevViewPort;
	ID3D11DepthStencilView* pPrevDSV;
	ID3D11RenderTargetView* pPrevRTV;
	ID3D11RasterizerState* pPrevRS;
	ID3D11SamplerState* pSamplerState;
	
	
	// 转换到光源为视点的坐标系
	Mat4 lightView;
	Mat4 lightProjection;
	Mat4 lightSpaceMatrix;
};