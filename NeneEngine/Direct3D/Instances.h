/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <memory>
#include <vector>

#include "Shape.h"
#include "Model.h"

//
//  使用Instancing绘制多个物体
//

class Instances {
public:
	Instances(std::unique_ptr<Shape> s, std::vector<Mat4> &models);
	Instances(std::unique_ptr<Model> s, std::vector<Mat4> &models);
	void draw(Shader *pShader, Camera *pCameras);
	~Instances();
private:
	Instances();
	Instances(const Instances &other);
	//
	std::unique_ptr<Shape> pShape;
	std::unique_ptr<Model> pModel;
	UINT instancesNum;
	UINT offset;
	UINT perInstanceSize;
	//
	ID3D11Buffer* pInstanceBuffer;
};