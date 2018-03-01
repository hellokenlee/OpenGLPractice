/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/

#include "Instances.h"

//!TODO: 应该为每个Drawable对象提供一个DrawInstances接口！
Instances::Instances(std::unique_ptr<Shape> s, std::vector<Mat4> &models) {
	// 初始化 InstanceBuffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Mat4) * models.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = models.data();
	HRESULT hr = Utils::getDevice()->CreateBuffer(&bufferDesc, &initData, &pInstanceBuffer);
	checkResult(hr, "Fail to instancing object!");
	//
	pShape = std::move(s);
	pModel = nullptr;
	instancesNum = models.size();
	perInstanceSize = sizeof(Mat4);
	offset = 0;
}

Instances::Instances(std::unique_ptr<Model> s, std::vector<Mat4> &models) {
	// 初始化 InstanceBuffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Mat4) * models.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = models.data();
	HRESULT hr = Utils::getDevice()->CreateBuffer(&bufferDesc, &initData, &pInstanceBuffer);
	checkResult(hr, "Fail to instancing object!");
	//
	pShape = nullptr;
	pModel = std::move(s);
	instancesNum = models.size();
	perInstanceSize = sizeof(Mat4);
	offset = 0;
}

Instances::~Instances() {
	if(pInstanceBuffer) pInstanceBuffer->Release();
}

void Instances::draw(Shader *pShader, Camera *pCamera) {
	// 如果着色器不为空， 使用该着色器
	if (pShader) pShader->use();
	if (pCamera) pCamera->use();
	Utils::getContext()->IASetVertexBuffers(1, 1, &(pInstanceBuffer), &perInstanceSize, &offset);
	if (pShape != nullptr) {
		// 设置顶点缓冲
		Utils::getContext()->IASetVertexBuffers(0, 1, &(pShape->pVertexBuffer), &(pShape->perVertexSize), &(pShape->offset));
		// 设置绘制模式
		Utils::getContext()->IASetPrimitiveTopology((pShape->drawMode));
		// 分模式绘制
		if (pShape->indicesNum == 0) {
			// 非索引模式
			Utils::getContext()->DrawInstanced(pShape->verticesNum, instancesNum, 0, 0);
		}
		else {
			// 索引模式
			Utils::getContext()->IASetIndexBuffer(pShape->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			Utils::getContext()->DrawIndexedInstanced(pShape->indicesNum, instancesNum, 0, 0, 0);
		}
	}
	if (pModel != nullptr) {
		Utils::getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		for (UINT i = 0; i < (pModel->meshes.size()); ++i) {
			// 使用这个网格的素材
			for (UINT j = 0; j < (pModel->meshes[i])->textures.size(); ++j) {
				(pModel->meshes[i])->textures[j]->use((pModel->meshes[i])->texTypes[j]);
			}
			// 设置顶点缓冲
			Utils::getContext()->IASetVertexBuffers(0, 1, &(pModel->meshes[i])->pVertexBuffer, &(pModel->meshes[i])->perVertexSize, &(pModel->meshes[i])->offset);
			// 索引模式绘制
			Utils::getContext()->IASetIndexBuffer((pModel->meshes[i])->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			Utils::getContext()->DrawIndexedInstanced((pModel->meshes[i])->indicesNum, instancesNum, 0, 0, 0);
		}
	}
	
}