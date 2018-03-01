/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <vector>
#include <memory>
#include "Types.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

// !NOTE: 网格对象的顶点格式永远是PNT
struct MeshVertexPNT {
	MeshVertexPNT() : position(0.0f, 0.0f, 0.0f), normal(0.0f, 1.0f, 0.0f), texCoord(0.0f, 0.0f) {}
	Vec3 position;
	Vec3 normal;
	Vec2 texCoord;
};

//
//  网格类
//

class Mesh {
public:
	friend class Instances;
	// 构造
	Mesh(std::vector<MeshVertexPNT> &vertices, std::vector<UINT> &indices, std::vector<std::shared_ptr<Texture> > &texture, std::vector<TextureType> &texType);
	// 绘制
	void draw(Shader* const shader = nullptr, Camera* const cam = nullptr);
private:
	// 禁止默认构造和拷贝构造
	Mesh();
	Mesh(const Mesh& other);
private:
	// 保存纹理对象
	std::vector<std::shared_ptr<Texture>> textures;
	std::vector<TextureType> texTypes;
	// 指向VBO的指针
	ID3D11Buffer *pVertexBuffer;
	// 索引缓冲
	ID3D11Buffer *pIndexBuffer;
	// 位置开始的偏移
	UINT offset;
	// 每个顶点的位宽
	UINT perVertexSize;
	//
	UINT indicesNum, verticesNum;
};