/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "TextureCube.h"
#include "Geometry.h"
#include "Shader.h"

//
//  天空盒类
//

class Skybox {
public:
	Skybox(const WCHAR* filePath[6]);
	Skybox(const WCHAR* ddsFilePath);
	~Skybox();
	void draw(Camera *pCamera = nullptr);
	void moveTo(Vec3 const pos);
private:
	Skybox();
	Skybox(const Skybox& other);
	//
	std::unique_ptr<Shape> sphere;
	TextureCube cubeTexture;
	Shader shader;
	//
	ID3D11DepthStencilState *pDSState;
	ID3D11DepthStencilState *pPrevDSState;
	UINT prevStencilRef;
};
