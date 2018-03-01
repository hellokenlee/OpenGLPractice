/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "Shape.h"

//
//	几何类(静态工厂类)
//

///!NOTE: 所有创建的Shape的顶点格式都是POSITIONS_NORMALS_TETXURES
class Geometry {
public:
	// 创建一个UV球体 (!FIXED ME: 未计算法线和纹理坐标, VertexFormat = POSITONS)
	static std::unique_ptr<Shape> createUVSphere(UINT const latLines, UINT const longLines, VertexOrder vo = COUNTER_CLOCK_WISE);
	// 穿件一个多面体球体
	static std::unique_ptr<Shape> createIcosphere(UINT const level = 3, VertexOrder vo = COUNTER_CLOCK_WISE);
	// 创建一个立方体
	static std::unique_ptr<Shape> createCube(VertexOrder vo = COUNTER_CLOCK_WISE);
	// 创建一个四边形
	static std::unique_ptr<Shape> createQuad(VertexOrder vo = COUNTER_CLOCK_WISE);
	// 创建一个圆锥
	static std::unique_ptr<Shape> createCone(VertexOrder vo = COUNTER_CLOCK_WISE);
	// 创建一个圆环
	static std::unique_ptr<Shape> createTorus(VertexOrder vo = COUNTER_CLOCK_WISE);
	// 生成法向量(平均法) !NOTE: 顶点一定要是PNT且索引必须是以TRIANGLE_LIST形式给出
	void calcNormals(std::vector<FLOAT> &vertices, std::vector<UINT> &indices);
private:
	// 创建icoSphere调用
	static UINT createIcosphere_GetMiddle(std::unordered_map<ULONGLONG, UINT> &vertexHash, std::vector<Vec3> &vertices, UINT index1, UINT index2);
	// 翻转顶点顺序(顺时针 <-> 逆时针)
	static void invertIndexOrder(UINT *indices, UINT iNum);
};