/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once
#include "Shader.h"
#include "Shape.h"
#include "Texture.h"
#include "ConstantBuffer.h"


//
//		发射器
//
struct Emitter {
	Emitter() {
		position = Vec3(0.0f, 0.0f, 0.0f);
		direction = Vec3(0.0f, 1.0f, 0.0f);
	};
	Emitter(Vec3 _pos, Vec3 _dir) {
		position = _pos;
		direction = _dir;
	}
	// 发射器位置
	Vec3 position;
	// 发射方向
	Vec3 direction;
};


//
//		粒子系统类(T为粒子结构体)
//
template<typename T> 
class ParticleSystem {
public:
	// 构造和析构
	ParticleSystem(const UINT particlesNum);
	~ParticleSystem();
	// 设置发射器
	void setEmitter(Emitter &emitter);
	// 重设该粒子系统
	void reset();
	// 使用Steam-Out Only， 在几何着色器中更新所有粒子的信息，然后回写进顶点缓冲
	void update(Shader *pShader, FLOAT dt);
	// 把(更新后的)粒子绘制出来
	void draw(Shader *pShader = nullptr, Camera *pCamera = nullptr);
private:
	// 禁止默认构造和析构
	ParticleSystem();
	ParticleSystem(const ParticleSystem &otehr);
	// 初始化顶点缓冲
	void initVertexBuffer(ID3D11Buffer* &pVertexBuffer, const T* emiterParticles = nullptr, const UINT emiterParticlesSize = 0);
private:
	// 时间
	FLOAT time;
	// 是否第一次绘制
	BOOL isFirstTime;
	// 发射器
	Emitter emitter;
	//
	UINT particlesNum;
	UINT perParticleSize;
	UINT vbOffset;
	//
	ID3D11Buffer* pInitVB;
	ID3D11Buffer* pDrawVB;
	ID3D11Buffer* pStreamOutVB;
	ID3D11Buffer* pEmptyBuffer[1];
	ID3D11DepthStencilState* pDSState;
	ID3D11BlendState* pBlendState;
	UINT soOffset;
	// 随机纹理
	std::unique_ptr<Texture> pRandomTex0;
	std::unique_ptr<Texture> pRandomTex1;
	// 保存之前的绘制状态
	UINT prevStencilRef;
	ID3D11DepthStencilState* pPrevDSState;
};


// ============================ 实现 ============================ //

template<typename T>
ParticleSystem<T>::ParticleSystem(const UINT _particlesNum) {
	// 初始化
	pDSState = nullptr;
	pPrevDSState = nullptr;
	pDrawVB = nullptr;
	pStreamOutVB = nullptr;
	pRandomTex0 = nullptr;
	pRandomTex1 = nullptr;
	prevStencilRef = 0;
	soOffset = 0;
	time = 0.0f;
	isFirstTime = true;
	pEmptyBuffer[0] = nullptr;
	particlesNum = _particlesNum;
	perParticleSize = sizeof(T);
	pBlendState = nullptr;
	// 准备Stream-Out Only的DS状态
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.StencilEnable = false;
	HRESULT hr1 = Utils::getDevice()->CreateDepthStencilState(&dsDesc, &pDSState);
	checkResult(hr1, "Fail to create particle system DS state.");
	// 准备顶点缓冲
	initVertexBuffer(pStreamOutVB);
	initVertexBuffer(pDrawVB);
	T emiterParticle;
	initVertexBuffer(pInitVB, &emiterParticle, 1);
	// 随机纹理
	pRandomTex0 = Texture::createRandomTexture1D(particlesNum);
	pRandomTex1 = Texture::createRandomTexture1D(particlesNum);
	//
	ConstantBuffer::perFrameCBData.emitterDir = emitter.direction;
	ConstantBuffer::perFrameCBData.emitterPos = emitter.position;
	// 混合
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;
	ZeroMemory(&rtbDesc, sizeof(rtbDesc));

	rtbDesc.BlendEnable = true;
	rtbDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbDesc.DestBlend = D3D11_BLEND_ONE;
	rtbDesc.BlendOp = D3D11_BLEND_OP_ADD;

	rtbDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbDesc;

	HRESULT hr2 = Utils::getDevice()->CreateBlendState(&blendDesc, &pBlendState);
	checkResult(hr2, "Failed to create blend state.");
}

template<typename T>
ParticleSystem<T>::~ParticleSystem() {
	if (pDrawVB) pDrawVB->Release();
	if (pStreamOutVB) pStreamOutVB->Release();
	if (pInitVB) pInitVB->Release();
	if (pDSState) pDSState->Release();
	if (pPrevDSState) pPrevDSState->Release();
	if (pBlendState) pBlendState->Release();
}

template<typename T>
void ParticleSystem<T>::reset() {
	time = 0.0f;
	isFirstTime = true;
}

template<typename T>
void ParticleSystem<T>::setEmitter(Emitter &_emitter) {
	emitter = _emitter;
	ConstantBuffer::perFrameCBData.emitterDir = emitter.direction;
	ConstantBuffer::perFrameCBData.emitterPos = emitter.position;
}

template<typename T>
void ParticleSystem<T>::initVertexBuffer(ID3D11Buffer* &pVertexBuffer, const T* emiterParticles, const UINT emiterParticlesNum) {
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(vbDesc));
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	if (emiterParticles == nullptr) {
		vbDesc.ByteWidth = sizeof(T) * particlesNum;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		HRESULT hr2 = Utils::getDevice()->CreateBuffer(&vbDesc, nullptr, &pVertexBuffer);
		checkResult(hr2, "Failed to create particle system vertices buffer.");
	} else {
		vbDesc.ByteWidth = sizeof(T) * emiterParticlesNum;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = emiterParticles;
		HRESULT hr2 = Utils::getDevice()->CreateBuffer(&vbDesc, &initData, &pVertexBuffer);
		checkResult(hr2, "Failed to create particle system vertices buffer.");
	}
	
}

template<typename T> void ParticleSystem<T>::update(Shader * pShader, FLOAT dt) {
	//
	time += dt;
	ConstantBuffer::perFrameCBData.dt = dt;
	ConstantBuffer::perFrameCBData.time = time;
	Utils::getContext()->GSSetConstantBuffers(SLOT_PER_FRAME_CB, 1, &(ConstantBuffer::pPerFrameConstantBuffer));
	// 保存之前的DS状态
	Utils::getContext()->OMGetDepthStencilState(&pPrevDSState, &prevStencilRef);
	// 使用Shader
	if (pShader) pShader->use();
	dLogIf(!pShader, "[Error] updating particle system with an empty shader!\n");
	// 只是StreamOut， 不渲染(1. 把PS置空; 2. 关闭深度/模板测试)
	Utils::getContext()->PSSetShader(nullptr, nullptr, 0);
	Utils::getContext()->OMSetDepthStencilState(pDSState, 0);
	// 设置顶点缓冲 和 绘制模式
	if (isFirstTime) {
		Utils::getContext()->IASetVertexBuffers(0, 1, &pInitVB, &perParticleSize, &vbOffset);
	} else {
		Utils::getContext()->IASetVertexBuffers(0, 1, &pDrawVB, &perParticleSize, &vbOffset);
	}
	Utils::getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	// 绑定到SO
	Utils::getContext()->SOSetTargets(1, &pStreamOutVB, &soOffset);
	// 使用随机纹理
	pRandomTex0->use(0);
	pRandomTex1->use(1);
	// 绘制(更新所有粒子)
	if (isFirstTime) {
		// 第一次绘制，先绘制一个顶点
		Utils::getContext()->Draw(1, 0);
		isFirstTime = false;
	} else {
		// 绘制全部
		Utils::getContext()->DrawAuto();
	}
	// 解除SO绑定
	Utils::getContext()->SOSetTargets(1, pEmptyBuffer, &soOffset);
	// 恢复之前的DS状态
	Utils::getContext()->OMSetDepthStencilState(pPrevDSState, prevStencilRef);
	// 切换
	std::swap(pDrawVB, pStreamOutVB);
}

template<typename T> void ParticleSystem<T>::draw(Shader* pShader, Camera* pCamera) {

	if (pShader) pShader->use();
	if (pCamera) pCamera->use();
	pRandomTex0->use(0);
	pRandomTex1->use(1);
	Utils::getContext()->GSSetConstantBuffers(SLOT_PER_FRAME_CB, 1, &(ConstantBuffer::pPerFrameConstantBuffer));
	static const FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Utils::getContext()->OMSetBlendState(pBlendState, blendFactor, 0xffffffff);
	// 设置顶点缓冲 和 绘制模式
	Utils::getContext()->IASetVertexBuffers(0, 1, &pDrawVB, &perParticleSize, &vbOffset);
	Utils::getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	// 绘制
	Utils::getContext()->DrawAuto();
	//
	Utils::getContext()->OMSetBlendState(0, 0, 0xffffffff);
}