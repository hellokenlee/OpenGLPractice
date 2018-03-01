/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <memory>

#include "TextureLoader\WICTextureLoader.h"

#include "Shape.h"

//
// 地形类(静态工厂类)
//

class Terrain {
public:
	// 从HeightMap图片中创建地形
	static std::unique_ptr<Shape> createFromFile(const WCHAR* heightMapFilePath,
		std::vector<FLOAT> &vertices, std::vector<UINT> &indices,
		const WCHAR* normalMapFilePath, FLOAT mapScale = 255.0f, 
		FLOAT xLength = 100.0f, FLOAT zLength  = 100.0f, Vec3 shift = Vec3(0.0f, 0.0f, 0.0f));
	// 随机生成地形
	static std::unique_ptr<Shape> createRandom();
private:
	Terrain();
	Terrain(const Terrain& other);
	//
	static std::vector<BYTE> readImageData(const WCHAR* heightMapFilePath, UINT &height, UINT &width);
};
