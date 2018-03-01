/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <vector>
#include "Utils.h"
#include "Types.h"
#include "Shader.h"
#include "Camera.h"
#include "ConstantBuffer.h"

//
//		形状类
//

class Shape {
public:
	friend class Instances;
	///!NOTE: 顶点逆时针方向为正方向
	// 构造
	Shape();
	Shape(FLOAT* vertices, UINT vArrayLen, VertexFormat vf);
	Shape(FLOAT* vertices, UINT vArrayLen, UINT* indices, UINT iArrayLen, VertexFormat vf);
	Shape(std::vector<FLOAT> vertices, VertexFormat vf) : Shape(vertices.data(), vertices.size(), vf) {}
	Shape(std::vector<FLOAT> vertices, std::vector<UINT> indices, VertexFormat vf) : Shape(vertices.data(), vertices.size(), indices.data(), indices.size(), vf) {}
	// 析构
	~Shape();
	// 绘制 
	void draw(Shader* const shader = nullptr, Camera* const cam = nullptr, BOOL isUpdateCB = true);
	// 设置位置
	void moveTo(Vec3 const position);
	// 设置缩放
	void scaleTo(Vec3 const scale);
	// 旋转
	void rotateX(FLOAT const rox);
	void rotateY(FLOAT const roy);
	void rotateZ(FLOAT const roz);
	// 设置绘制模式
	void setDrawMode(D3D11_PRIMITIVE_TOPOLOGY drawMode);
public:
	// Model矩阵
	Mat4 model;
private:
	// 禁止拷贝构造
	Shape(const Shape& other);
	// 指向VBO的指针
	ID3D11Buffer *pVertexBuffer;
	// 定点数
	UINT verticesNum;
	// 顶点格式
	VertexFormat vertexFormat;
	// 每个顶点的位宽
	UINT perVertexSize;
	// 位置开始的偏移
	UINT offset;
	// 索引数量，为0时则非索引模式
	UINT indicesNum;
	// 索引缓冲
	ID3D11Buffer *pIndexBuffer;
	// 绘制模式
	D3D11_PRIMITIVE_TOPOLOGY drawMode;
	// 
	Vec3 scale;
	Vec3 position;
	Vec3 rotation;
};