/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Mesh.h"

Mesh::Mesh(std::vector<MeshVertexPNT> &vertices, std::vector<UINT> &indices, 
	std::vector<std::shared_ptr<Texture> > &_textures, std::vector<TextureType> &_texTypes) {
	// 储存纹理变量
	textures = _textures;
	texTypes = _texTypes;
	// 成员初始化数据
	pVertexBuffer = nullptr;                                   // 顶点缓冲初始化
	pIndexBuffer = nullptr;                                    // 索引缓冲初始化
	offset = 0;                                                // 偏移为0
	indicesNum = indices.size();                               // 计算索引数量
	perVertexSize = POSITIONS_NORMALS_TEXTURES * sizeof(FLOAT);// 计算每个顶点所占位宽
	verticesNum = vertices.size();                             // 计算顶点数量
	// 初始化索引缓冲描述符
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * indicesNum;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// 初始化索引数据
	D3D11_SUBRESOURCE_DATA iInitData;
	ZeroMemory(&iInitData, sizeof(iInitData));
	iInitData.pSysMem = indices.data();
	// 写入到索引缓冲
	HRESULT hr = S_OK;
	hr = Utils::getDevice()->CreateBuffer(&indexBufferDesc, &iInitData, &pIndexBuffer);
	checkResult(hr, "Failded to create index buffer.");
	// 初始化顶点缓冲描述符
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = perVertexSize * verticesNum;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// 初始化顶点数据
	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vertices.data();
	// 写入到顶点缓冲
	hr = Utils::getDevice()->CreateBuffer(&vertexBufferDesc, &vInitData, &pVertexBuffer);
	checkResult(hr, "Failded to create vertex buffer.");
}

void Mesh::draw(Shader * const shader, Camera * const cam) {
	if (shader) shader->use();
	if (cam) cam->use();
	// 使用这个网格的素材
	for (UINT i = 0; i < textures.size(); ++i) {
		textures[i]->use(texTypes[i]);
	}
	// 设置顶点缓冲
	Utils::getContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &perVertexSize, &offset);
	// 索引模式绘制
	Utils::getContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Utils::getContext()->DrawIndexed(indicesNum, 0, 0);
}
