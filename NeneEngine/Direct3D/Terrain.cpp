/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Terrain.h"
using namespace std;

unique_ptr<Shape> Terrain::createFromFile(const WCHAR * heightMapFilePath,
	std::vector<FLOAT> &terrainVertices, std::vector<UINT> &terrainIndices,
	const WCHAR* normalMapFilePath, FLOAT mapScale, FLOAT xLength, FLOAT zlength , Vec3 shift) {
	//
	UINT hmHeight, hmWidth;
	vector<BYTE> heightMapImage = readImageData(heightMapFilePath, hmHeight, hmWidth);
	UINT nmHeight, nmWidth;
	vector<BYTE> normalMapImage = readImageData(normalMapFilePath, nmHeight, nmWidth);
	//
	assert(nmHeight == hmHeight && nmWidth == hmWidth);
	UINT height = nmHeight, width = nmWidth;
	// POSITION_NORMALS_TEXTURES
	terrainVertices.resize(width * height * POSITIONS_NORMALS_TEXTURES);
	dLog("[Info] Generating Height Map Vertices...");
	// 填充顶点
	FLOAT y, r, g, b;
	UINT k = 0, index = 0;
	FLOAT xScale = xLength / (FLOAT)width, zScale = zlength / (FLOAT)height;
	for (UINT z = 0; z < height; ++z) {
		for (UINT x = 0; x < width; ++x) {
			// 高度
			y = (FLOAT)(heightMapImage[k]);
			index = (z * width + x) * POSITIONS_NORMALS_TEXTURES;
			// POSITIONS_XYZ
			terrainVertices[index + 0] = ((FLOAT)x - (width / 2.0f)) * xScale + shift.x;
			terrainVertices[index + 1] = (y / 255.0f) * mapScale + shift.y;
			terrainVertices[index + 2] = ((FLOAT)z - (height / 2.0f)) * zScale + shift.z;
			// TEXTURES_UV
			terrainVertices[index + 6] = terrainVertices[index + 0] * 0.10f;
			terrainVertices[index + 7] = terrainVertices[index + 2] * 0.10f;
			// NORMALS_XYZ
			b = (FLOAT)(normalMapImage[k + 0]);
			g = (FLOAT)(normalMapImage[k + 1]);
			r = (FLOAT)(normalMapImage[k + 2]);
			// tangent space -> world sapce
			terrainVertices[index + 3] = r / 255.0f;
			terrainVertices[index + 4] = b / 255.0f;
			terrainVertices[index + 5] = -g / 255.0f;

			//
			k += 4;
		}
	}
	dLog("Done!\n[Info] Generating Height Map Indices...");
	//  height = 3, width = 3; A = 10; B = 11;
	//  0 -- 1(B) -- A
	//    /        /
	//  2 -- 3(9) -- 8
	//    /        /
	//  4 -- 5(7) -- 6 
	for (UINT j = 0; j < width - 1;) {
		for (UINT i = 0; i < height; ++i) {
			terrainIndices.push_back(i * width + j);
			terrainIndices.push_back(i * width + j + 1);
		}
		++j;
		if (j >= width - 1) break;
		for (int i = height - 1; i >= 0; --i) {
			terrainIndices.push_back(i * width + j + 1);
			terrainIndices.push_back(i * width + j);
		}
		++j;
	}
	dLog("Done!\n");
	//
	//
	unique_ptr<Shape> res = unique_ptr<Shape>(new Shape(terrainVertices, terrainIndices, POSITIONS_NORMALS_TEXTURES));
	res->setDrawMode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	return res;
}

vector<BYTE> Terrain::readImageData(const WCHAR* heightMapFilePath, UINT &height, UINT &width) {
	// 检查
	checkFileExist(heightMapFilePath);
	// 载入纹理
	ID3D11Resource* pTexture;
	HRESULT hr1 = DirectX::CreateWICTextureFromFileEx(Utils::getDevice(), heightMapFilePath, 0,
		D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_READ, 0, DirectX::WIC_LOADER_DEFAULT, &pTexture, nullptr);
	checkResult(hr1, "Failed to create texture.");
	// 获取纹理描述符
	D3D11_TEXTURE2D_DESC texDesc;
	((ID3D11Texture2D*)pTexture)->GetDesc(&texDesc);
	// 映射到内存
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	HRESULT hr2 = Utils::getContext()->Map(pTexture, 0, D3D11_MAP_READ, 0, &mappedBuffer);
	checkResult(hr1, "Failed to map texture.");
	// !TODO: 处理其他可能的格式： DXGI_FORMAT_R8_UNORM，DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R8G8B8A8_UNORM
	assert(texDesc.Format == DXGI_FORMAT_B8G8R8A8_UNORM || texDesc.Format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
	// 拷贝数据
	height = texDesc.Height;
	width = texDesc.Width;
	UINT dataSize = height * width * sizeof(BYTE) * 4;
	vector<BYTE> imageData(dataSize);
	memcpy(imageData.data(), mappedBuffer.pData, dataSize);
	// 释放纹理
	pTexture->Release();
	//
	return imageData;
}