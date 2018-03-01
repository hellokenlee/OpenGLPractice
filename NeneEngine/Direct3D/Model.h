/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <memory>
#include <unordered_map>

#include "Assimp\Importer.hpp"
#include "Assimp\scene.h"
#include "Assimp\postprocess.h"

#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

#pragma comment (lib, "assimp.lib")

//
//  模型类
//

class Model {
public:
	friend class Instances;
	// 构造
	Model(const WCHAR* filepath);
	// 析构
	~Model();
	// 绘制
	void draw(Shader* const shader = nullptr, Camera* const cam = nullptr);
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
private:
	void loadModel(const WCHAR* filepath);
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	void loadMaterialTextures(aiMaterial* mat, aiTextureType typeAssimp, TextureType typeNene, 
		std::vector<std::shared_ptr<Texture> > &texture, std::vector<TextureType> &texType);
	std::string ws2s(const std::wstring& s);
	std::wstring s2ws(const std::string& s);
private:
	// 禁止默认构造和拷贝
	Model();
	Model(const Model &other);
	// 模型文件目录
	std::string directory;
	// 网格模型
	std::vector<std::unique_ptr<Mesh>> meshes;
	// 缓存已经加载过的纹理
	std::unordered_map<std::string, std::shared_ptr<Texture> > cachedTextures;
	// Model矩阵
	Mat4 model;
	// 位置
	Vec3 scale;
	// 缩放
	Vec3 position;
	//
	Vec3 rotation;
	// 绘制模式
	D3D11_PRIMITIVE_TOPOLOGY drawMode;
};