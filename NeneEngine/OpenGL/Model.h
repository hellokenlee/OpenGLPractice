/*Copyright reserved by KenLee@2018 ken4000kl@gmail.com*/
#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Camera.h"
#include "Drawable.h"

#include <memory>
#include <unordered_map>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//
//	模型类
//

class Model : public Drawable {
public:
    // 构造函数
    Model(const Char* filepath);
    ~Model();
    // 绘制
    virtual void draw(const Shader* pShader = nullptr, const Camera* pCamera = nullptr);
    virtual void drawInstanced(const Shader* pShader = nullptr, const Camera* pCamera = nullptr);
private:
    // 禁止拷贝
    Model();
    Model(const Model& rhs);
    Model& operator=(const Model& rhs);
    // 网格
    std::vector<std::unique_ptr<Mesh>> meshes;
    // 缓存已经加载过的纹理
	std::unordered_map<std::string, std::shared_ptr<Texture>> cachedTextures;
    // 模型所在目录
    std::string directory;
    // 辅助函数
    void loadModel(std::string path);
    void processNode(const aiNode* node, const aiScene* scene);
    void processMesh(const aiMesh* mesh, const aiScene* scene);
    void loadMaterialTextures(aiMaterial* mat, aiTextureType typeAssimp, TextureType typeNene,
                              std::vector<std::shared_ptr<Texture>> &texture,
                              std::vector<TextureType> &texType);
};

#endif	//	MODEL_H
