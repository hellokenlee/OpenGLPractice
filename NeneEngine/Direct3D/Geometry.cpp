/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Geometry.h"

using namespace DirectX::SimpleMath;

struct UVec3 {
	UVec3(UINT _x, UINT _y, UINT _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	UINT x, y, z;
};

std::unique_ptr<Shape> Geometry::createIcosphere(UINT const level, VertexOrder vo) {
	//
	assert(sizeof(UINT) == 4 && sizeof(ULONGLONG) == 8);
	// 顶点数组
	std::vector<Vec3> vertices; 
	FLOAT t = (1.0f + sqrtf(5.0f)) / 2.0f;
	FLOAT len = sqrt(1.0f + (t * t));
	t /= len;
	FLOAT s = 1.0f / len;
	// 3个互相垂直的长方形
	// XoY平面上
	vertices.push_back(Vec3(-s, t, 0.0f));
	vertices.push_back(Vec3(s, t, 0.0f));
	vertices.push_back(Vec3(-s, -t, 0.0f));
	vertices.push_back(Vec3(s, -t, 0.0f));
	// YoZ平面上
	vertices.push_back(Vec3(0.0f, -s, t));
	vertices.push_back(Vec3(0.0f, s, t));
	vertices.push_back(Vec3(0.0f, -s, -t));
	vertices.push_back(Vec3(0.0f, s, -t));
	// XoZ平面上
	vertices.push_back(Vec3(t, 0.0f, -s));
	vertices.push_back(Vec3(t, 0.0f, s));
	vertices.push_back(Vec3(-t, 0.0f, -s));
	vertices.push_back(Vec3(-t, 0.0f, s));
	
	// 索引数组
	std::vector<UVec3> faces;
	// 根据上面的12个顶点构造20面体(逆时针顺序)
	// 0顶点相邻的5个三角面
	faces.push_back(UVec3( 0,  5, 11));
	faces.push_back(UVec3( 0,  1,  5));
	faces.push_back(UVec3( 0,  7,  1));
	faces.push_back(UVec3( 0, 10,  7));
	faces.push_back(UVec3( 0, 11, 10));
	//
	faces.push_back(UVec3( 1,  9,  5));
	faces.push_back(UVec3( 5,  4, 11));
	faces.push_back(UVec3(11,  2, 10));
	faces.push_back(UVec3(10,  6,  7));
	faces.push_back(UVec3( 7,  8,  1));
	// 3顶点附近的5个三角面
	faces.push_back(UVec3( 3,  4, 9));
	faces.push_back(UVec3( 3,  2, 4));
	faces.push_back(UVec3( 3,  6, 2));
	faces.push_back(UVec3( 3,  8, 6));
	faces.push_back(UVec3( 3,  9, 8));
	//
	faces.push_back(UVec3( 4,  5,  9));
	faces.push_back(UVec3( 2, 11,  4));
	faces.push_back(UVec3( 6, 10,  2));
	faces.push_back(UVec3( 8,  7,  6));
	faces.push_back(UVec3( 9,  1,  8));

	// 细分
	std::unordered_map<ULONGLONG, UINT> verticesHash;
	for (UINT i = 0; i < level; ++i) {
		std::vector<UVec3> faces2;
		for (unsigned int f = 0; f < faces.size(); ++f) {
			// 原三角形的三个顶点编号
			int p1 = faces[f].x;
			int p2 = faces[f].y;
			int p3 = faces[f].z;
			int a = createIcosphere_GetMiddle(verticesHash, vertices, p1, p2);
			int b = createIcosphere_GetMiddle(verticesHash, vertices, p2, p3);
			int c = createIcosphere_GetMiddle(verticesHash, vertices, p3, p1);
			// abc为新的内接三角形： 一个大三角形P1P2P3 -> 四个小三角形(保持逆时针)
			faces2.push_back(UVec3(p1, a, c));
			faces2.push_back(UVec3(p2, b, a));
			faces2.push_back(UVec3(p3, c, b));
			faces2.push_back(UVec3( a, b, c));
		}
		faces = faces2;// 细分后的面取代原来的面
	}
	if (vo == CLOCK_WISE) {
		invertIndexOrder(&faces[0].x, faces.size() * 3);
	}
	dLog("[Info] Create icosphere with level %d :\n", level);
	dLog("    Vertices Num: %d \n", vertices.size());
	dLog("    Faces Num: %d \n", faces.size());
	// 计算纹理坐标
	std::vector<FLOAT> verticesPNT;
	for (UINT f = 0; f < faces.size(); ++f) {
		FLOAT x[3], y[3], z[3];
		FLOAT u[3], v[3];
		bool isPole = false;
		int poleVertex = -1;
		for (UINT i = 0; i < 3; ++i) {
			int index = *(&(faces[f].x) + i);
			x[i] = vertices[index].x;
			y[i] = vertices[index].y;
			z[i] = vertices[index].z;
			// 计算法纹理坐标
			u[i] = 0.5f + (atan2(-z[i], -x[i]) / M_PI / 2.0f);
			v[i] = 0.5f - (asin(-y[i]) / M_PI);
			// 检测是否极点
			if (x[i] == 0.0 && z[i] == 0.0 && (y[i] == 1.0 || y[i] == -1.0)) {
				poleVertex = i;
				isPole = true;
			}
		}
		//修复球体边上的锯齿状纹理错误
		Vec3 tmp = Vec3(u[2], v[2], 0.0f) - Vec3(u[0], v[0], 0.0f);
		Vec3 texNormal = -tmp.Cross(Vec3(u[1], v[1], 0.0f) - Vec3(u[0], v[0], 0.0f));
		if (texNormal.z < 0) {
			//dLog("    Triangle (%.2f, %.2f, %.2f) - (%.2f, %.2f, %.2f) - (%.2f, %.2f, %.2f):\n ", x[0], y[0], z[0], x[1], y[1], z[1], x[2], y[2], z[2]);
			//dLog("    Have UV:(%.2f, %.2f) , (%.2f, %.2f) , (%.2f, %.2f): \n", u[0], v[0], u[1], v[1], u[2], v[2]);
			for (int i = 0; i < 3; ++i) {
				if (u[i] < 0.5)
					u[i] += 1.0;
			}
			//dLog("    Fixed UV:(%.2f, %.2f) , (%.2f, %.2f) , (%.2f, %.2f): \n\n", u[0], v[0], u[1], v[1], u[2], v[2]);
		}
		if (isPole) {
			u[poleVertex] = (u[(poleVertex + 1) % 3] + u[(poleVertex + 2) % 3]) / 2;
		}
		//
		for (int i = 0; i < 3; ++i) {
			// POSITION
			verticesPNT.push_back(x[i]);
			verticesPNT.push_back(y[i]);
			verticesPNT.push_back(z[i]);
			// NORMAL 球体的点的法向量等于点的坐标(因为球心为原点)
			verticesPNT.push_back(x[i]);
			verticesPNT.push_back(y[i]);
			verticesPNT.push_back(z[i]);
			// TEXTURES
			verticesPNT.push_back(u[i]);
			verticesPNT.push_back(v[i]);
		}
	}
	dLog("    Total Render vertices number(without indexing): %d \n\n", verticesPNT.size() / 8);
	return std::unique_ptr<Shape>(new Shape(verticesPNT, POSITIONS_NORMALS_TEXTURES));
}


void Geometry::invertIndexOrder(UINT *indices, UINT iNum) {
#ifdef DEBUG
	if (iNum % 3 != 0) {
		printf("[Error] Inverting vertex order of a non-trianglized mesh may cause undefine behavior.\n");
	}
#endif
	for (UINT v = 0; v < iNum / 3; ++v) {
		std::swap(indices[3 * v + 1], indices[3 * v + 2]);
	}
}

UINT Geometry::createIcosphere_GetMiddle(std::unordered_map<ULONGLONG, UINT> &vertexHash, std::vector<Vec3> &vertices, UINT index1, UINT index2) {
	if (index1 > index2) {
		// 保证index1 < index2
		std::swap(index1, index2);
	}
	ULONGLONG key = ((ULONGLONG)index1 << 32) + index2;
	if (vertexHash.find(key) != vertexHash.end()) {
		// 如果已经计算过了该个点
		return vertexHash[key];
	}
	// 如果没有计算过这个点，新生成一个点
	Vec3 newVertex((vertices[index1].x + vertices[index2].x) / 2,
		(vertices[index1].y + vertices[index2].y) / 2,
		(vertices[index1].z + vertices[index2].z) / 2);
	
	FLOAT len = sqrtf((newVertex.x * newVertex.x) + (newVertex.y * newVertex.y) + (newVertex.z * newVertex.z));
	// 单位化处理
	newVertex.x /= len;
	newVertex.y /= len;
	newVertex.z /= len;
	// 把新的顶点放入顶点集
	int res = vertices.size();
	vertices.push_back(newVertex);
	vertexHash[key] = res;
	//
	return res;
};

std::unique_ptr<Shape> Geometry::createUVSphere(UINT const latLines, UINT const longLines, VertexOrder vo) {
		// 定点数
		UINT vNum = ((latLines - 2) * longLines) + 2;
		// 面数
		UINT fNum = ((latLines - 3) * (longLines) * 2) + (longLines * 2);
		//
		FLOAT sphereYaw = 0.0f;
		FLOAT spherePitch = 0.0f;
		// 初始化顶点数据
		std::vector<Vec3> vertices(vNum);
		Vec3 currVertPos = Vec3(0.0f, 0.0f, 1.0f);
		vertices[0] = currVertPos;
		// 逐顶点计算位置
		for (UINT i = 0; i < latLines - 2; ++i) {
			spherePitch = (i + 1) * (M_PI / (latLines - 1));
			Mat4 Rotationx = Matrix::CreateRotationX(spherePitch);
			for (UINT j = 0; j < longLines; ++j) {
				sphereYaw = j * (M_PI_TIMES_2 / longLines);
				Mat4 Rotationy = Matrix::CreateRotationZ(sphereYaw);
				currVertPos = Vector3::TransformNormal(Vec3(0.0f, 0.0f, 1.0f), (Rotationx * Rotationy));
				currVertPos.Normalize();
				vertices[i * longLines + j + 1] = currVertPos;
			}
		}
		// 收尾
		vertices[vNum - 1] = Vec3(0.0f, 0.0f, -1.0f);
		
		// 准备索引数据(把四边形拆分成两个三角形)
		std::vector<UINT> indices(fNum * 3);
		int k = 0;
		for (UINT l = 0; l < longLines - 1; ++l) {
			indices[k] = 0;
			indices[k + 1] = l + 2;
			indices[k + 2] = l + 1;
			k += 3;
		}
		indices[k] = 0;
		indices[k + 1] = 1;
		indices[k + 2] = longLines;
		k += 3;
		//
		for (UINT i = 0; i < latLines - 3; ++i) {
			for (UINT j = 0; j < longLines - 1; ++j) {
				indices[k] = i * longLines + j + 1;
				indices[k + 1] = i * longLines + j + 2;
				indices[k + 2] = (i + 1) * longLines + j + 1;
				//
				indices[k + 3] = (i + 1) * longLines + j + 1;
				indices[k + 4] = i * longLines + j + 2;
				indices[k + 5] = (i + 1) * longLines + j + 2;
				// 下一个四边形
				k += 6;
			}
			indices[k] = (i * longLines) + longLines;
			indices[k + 1] = (i * longLines) + 1;
			indices[k + 2] = ((i + 1) * longLines) + longLines;
			//
			indices[k + 3] = ((i + 1) * longLines) + longLines;
			indices[k + 4] = (i * longLines) + 1;
			indices[k + 5] = ((i + 1) * longLines) + 1;
			// 下一个四边形
			k += 6;
		}
		//
		for (UINT l = 0; l < longLines - 1; ++l) {
			indices[k] = vNum - 1;
			indices[k + 1] = (vNum - 1) - (l + 2);
			indices[k + 2] = (vNum - 1) - (l + 1);
			k += 3;
		}
		//
		indices[k] = vNum - 1;
		indices[k + 1] = vNum - 2;
		indices[k + 2] = (vNum - 1) - longLines;
		dLog("[Info] Create uv-sphere with %d latlines, %d longlines:\n", latLines, longLines);
		dLog("    Vertices Num: %d \n", vertices.size());
		dLog("    Faces Num: %d \n", indices.size() / 3);
		dLog("    Total Render vertices number: %d \n\n", indices.size());
		//
		if (vo == CLOCK_WISE) {
			invertIndexOrder(indices.data(), indices.size());
		}
		//
		return std::unique_ptr<Shape>(new Shape(&(vertices[0].x), vertices.size() * 3, &indices[0], indices.size(), POSITIONS));
}


std::unique_ptr<Shape> Geometry::createCube(VertexOrder vo) {
	static std::vector<FLOAT> vertices = {
		// POSITION_XYZ         NORMAL_XYZ          TEXTURE_UV
		//前面
		-1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 
		-1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		// 后面
		-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,   0.0f, 0.0f,-1.0f,   1.0f, 0.0f,
		// 上面
		-1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		// 下面
		-1.0f, -1.0f, -1.0f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,   0.0f,-1.0f, 0.0f,   1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f,
		// 左边
		-1.0f, -1.0f, -1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,  -1.0f, 0.0f, 0.0f,   1.0f, -1.0f,
		// 右边
		 1.0f, -1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f
	};
	static std::vector<UINT> indices = {
		// Front Face
		0,  1,  2,
		0,  2,  3,
		// Back Face
		4,  5,  6,
		4,  6,  7,
		// Top Face
		8, 10,  9,
		8, 11, 10,
		// Bottom Face
		12, 13, 14,
		12, 14, 15,
		// Left Face
		16, 18, 17,
		16, 19, 18,
		// Right Face
		20, 21, 22,
		20, 22, 23
	};
	if (vo == CLOCK_WISE) {
		invertIndexOrder(indices.data(), indices.size());
	}
	return std::unique_ptr<Shape>(new Shape(vertices, indices, POSITIONS_NORMALS_TEXTURES));
}

std::unique_ptr<Shape> Geometry::createQuad(VertexOrder vo) {
	std::vector<FLOAT> vertices = {
		// POSITION-XYZ        NORMAL-XYZ        TEXTURE-UV
		 1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f
	};
	std::vector<UINT> indices = {
		0, 2, 1,
		0, 3, 2
	};
	if (vo == CLOCK_WISE) {
		invertIndexOrder(indices.data(), indices.size());
	}
	return std::unique_ptr<Shape>(new Shape(vertices, indices, POSITIONS_NORMALS_TEXTURES));
}



void Geometry::calcNormals(std::vector<FLOAT> &vertices, std::vector<UINT> &indices) {
	dLog("[Info] Generating Normals...\n");
	// 初始化
	std::vector<Vec3> faceNormals;
	UINT facesNum = indices.size() / 3, verticesNum = vertices.size() / POSITIONS_NORMALS_TEXTURES;
	FLOAT x, y, z;
	Vec3 edge1, edge2, trivalNormal;
	// 遍历每个面， 求法向量
	for (UINT i = 0; i < facesNum; ++i) {
		// 获取表示一条边的向量 (edge 0,2)
		x = vertices[indices[(i * 3)] + 0] - vertices[indices[(i * 3) + 2] + 0];
		y = vertices[indices[(i * 3)] + 1] - vertices[indices[(i * 3) + 2] + 1];
		z = vertices[indices[(i * 3)] + 2] - vertices[indices[(i * 3) + 2] + 2];
		edge1 = Vec3(x, y, z);
		// 获取表示一条边的向量 (edge 2,1)
		x = vertices[indices[(i * 3) + 2] + 0] - vertices[indices[(i * 3) + 1] + 0];
		y = vertices[indices[(i * 3) + 2] + 1] - vertices[indices[(i * 3) + 1] + 1];
		z = vertices[indices[(i * 3) + 2] + 2] - vertices[indices[(i * 3) + 1] + 2];
		edge2 = Vec3(x, y, z);
		// 叉乘求面的法向量(!NOTE: 未单位化)
		trivalNormal = edge1.Cross(edge2);
		// 保存该面的向量
		faceNormals.push_back(trivalNormal);
	}
	//
	Vec3 normalSum;
	int facesUsing;
	// 一个点的法向量为包含这个点的面的法向量的平均
	for (UINT i = 0; i < verticesNum; ++i) {
		normalSum = Vec3(0.0f, 0.0f, 0.0f);
		facesUsing = 0;
		// 检测那些面用到了这个点
		for (UINT j = 0; j < facesNum; ++j) {
			if (indices[j * 3] == i || indices[(j * 3) + 1] == i || indices[(j * 3) + 2] == i) {
				normalSum.x = faceNormals[j].x;
				normalSum.y = faceNormals[j].y;
				normalSum.z = faceNormals[j].z;
				facesUsing++;
			}
		}
		// 求平均
		normalSum = normalSum / (FLOAT)facesUsing;
		// 单位化
		normalSum.Normalize();
		// 写入顶点法向量 - NORMALS_XYZ
		vertices[i * POSITIONS_NORMALS_TEXTURES + 3] = normalSum.x;
		vertices[i * POSITIONS_NORMALS_TEXTURES + 4] = normalSum.y;
		vertices[i * POSITIONS_NORMALS_TEXTURES + 5] = normalSum.z;
	}
	dLog("Done!\n");
}