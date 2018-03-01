/*Copyright reserved by KenLee@2018 ken4000kl@gmail.com*/
#include "Model.h"

using namespace std;

Model::Model(const GLchar* filepath) : Drawable() {
	// 初始化
	directory = "";
	meshes.clear();
	cachedTextures.clear();
	loadModel(filepath);
}

Model::~Model() {

}

void Model::draw(const Shader* pShader, const Camera* pCamera) {
    //
    if(pShader != nullptr) {
        pShader->use();
        glUniformMatrix4fv(glGetUniformLocation(pShader->programID, "model"), 1, GL_FALSE, glm::value_ptr(getModelMat()));
        if(pCamera != nullptr) {
            glUniformMatrix4fv(glGetUniformLocation(pShader->programID, "view"), 1, GL_FALSE, pCamera->getViewMatrixVal());
            glUniformMatrix4fv(glGetUniformLocation(pShader->programID, "projection"), 1, GL_FALSE, pCamera->getProjectionMatrixVal());
        }
    }
    //
    for(UInt i = 0; i < meshes.size(); ++i) {
        meshes[i]->draw(pShader);
    }
}

void Model::drawInstanced(const Shader* pShader, const Camera* pCamera) {

}

void Model::loadModel(string filepath) {
	//
	checkFileExist(filepath.c_str());
	//载入器
	Assimp::Importer importer;
	//场景指针，执行三角化操作、翻转纹理坐标
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
	//错误检测
	if (scene == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
		printf("[Error] Model Loading Error: %s\n\n", importer.GetErrorString());
		return;
	}
	// 记录文件目录以加载相对路径文件(如纹理)
	size_t pos = filepath.rfind('/');
	directory = filepath.substr(0, pos + 1);
	//
	dLog("[Info] ===== Loading model with total %d meshes: ===== \n", scene->mNumMeshes);
	// 从根节点开始遍历加载模型
	processNode(scene->mRootNode, scene);
	//
	dLog("[Info] ===== Model loading finished. ===== \n\n");
}


void Model::processNode(const aiNode* node, const aiScene* scene) {
	//
	ddLog("    |-- Load %d Meshes from a node.\n", node->mNumMeshes);
	// 生成当前节点的所有Mesh
	for (UInt i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	// 递归处理子节点
	for (UInt i = 0; i < node->mNumChildren; ++i) {
		this->processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(const aiMesh* mesh, const aiScene* scene) {
	// 构造Mesh需要的数据
	vector<MeshVertex> vertices;
	vector<UInt> indices;
	//
	ddLog("        |-- Load %d Vertices from a mesh.\n", mesh->mNumVertices);
	// 处理顶点数据
	for (UInt i = 0; i < mesh->mNumVertices; ++i) {
		MeshVertex vertex;
		Vec3 tmpVec3;
		Vec2 tmpVec2 = Vec2(0.0f, 0.0f);
		// 位置
		tmpVec3.x = mesh->mVertices[i].x;
		tmpVec3.y = mesh->mVertices[i].y;
		tmpVec3.z = mesh->mVertices[i].z;
		vertex.position = tmpVec3;
		// 法线
		if (mesh->mNormals) {
			tmpVec3.x = mesh->mNormals[i].x;
			tmpVec3.y = mesh->mNormals[i].y;
			tmpVec3.z = mesh->mNormals[i].z;
			vertex.normal = tmpVec3;
		}
		// 纹理
		if (mesh->mTextureCoords[0]) {
			tmpVec2.x = mesh->mTextureCoords[0][i].x;
			tmpVec2.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = tmpVec2;
		}
		// 把顶点放进去
		vertices.push_back(vertex);
	}
	// 处理索引数据(由于开启了三角化，所以mNumIndices事实上永远为3)
	for (UInt i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (UInt j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	// 纹理类型映射: ASSIMP纹理类型 -> Nene纹理类型
    vector<shared_ptr<Texture>> textures;
	vector<TextureType> texTypes;
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		loadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE, textures, texTypes);
		loadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_SPECULAR, textures, texTypes);
		loadMaterialTextures(material, aiTextureType_AMBIENT, TEXTURE_REFLECT, textures, texTypes);
		//loadMaterialTextures(material, aiTextureType_HEIGHT, TEXTURE_NORMAL, textures, texTypes);// 不知道为啥 assimp 把map_bump 解析成heightmap
		//loadMaterialTextures(material, aiTextureType_LIGHTMAP, TEXTURE_AMBIENT_OCCLUSION, textures, texTypes);
	}
	// Debug 输出
	ddLog("        |-- Process Mesh with:\n");
	ddLog("            Normal? : %s.\n", mesh->mNormals ? "Yes" : "No");
	ddLog("            Vertices: %d\n", vertices.size());
	ddLog("            Indices : %d\n", indices.size());
	ddLog("            Textures: %d\n", textures.size());
	// 把生成的网格对象压入成员变量
	meshes.push_back(unique_ptr<Mesh> (new Mesh(vertices, indices, textures, texTypes)));
}

void Model::loadMaterialTextures(aiMaterial* material, aiTextureType typeAssimp,
	TextureType typeNene, vector<shared_ptr<Texture>> &textures, vector<TextureType> &texTypes) {
	// 一次性读取同类型的纹理
	for (UInt i = 0; i < material->GetTextureCount(typeAssimp); ++i) {
		// 获取纹理的文件名
		aiString texFileName;
		material->GetTexture(typeAssimp, i, &texFileName);
		// 变成纹理的文件路径
		string texFilePath = directory + texFileName.C_Str();
		// 检测之前是否读取过
		auto it = cachedTextures.find(texFilePath);
		if (it == cachedTextures.end()) {
			// 之前没读取过, 生成新的纹理对象
			shared_ptr<Texture> pNewTexture(new Texture(texFilePath.c_str(), GL_BGRA, GL_RGBA));
			cachedTextures.insert(make_pair(texFilePath, pNewTexture));
			textures.push_back(pNewTexture);
			ddLog("            loaded new texture file[type: %d]: %s\n", typeNene, texFilePath.c_str());
		} else {
			// 之前已经读取过，避免重复读取
			textures.push_back(it->second);
			ddLog("            loaded cached texture file[type: %d]: %s\n", typeNene, texFilePath.c_str());
		}
		// 写入该纹理的类型
		texTypes.push_back(typeNene);
	}
}
