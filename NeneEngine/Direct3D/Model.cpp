/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/

#include "Model.h"

using namespace std;
using namespace DirectX::SimpleMath;

Model::Model(const WCHAR * filepath) : scale(1.0f, 1.0f, 1.0f), position(0.0f, 0.0f, 0.0f),
	rotation(0.0f, 0.0f, 0.0f), model(Mat4Identity), 
	drawMode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
	//
	meshes.clear();
	cachedTextures.clear();
	loadModel(filepath);
}

Model::~Model() {

}

void Model::draw(Shader * const shader, Camera * const cam) {
	//
	if (shader) shader->use();
	if (cam) cam->use();
	// 更新Model矩阵
	ConstantBuffer::perObjectCBData.model = model.Transpose();
	ConstantBuffer::updatePerObject();
	//
	Utils::getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 绘制所有网格
	for (UINT i = 0; i < meshes.size(); ++i) {
		meshes[i]->draw();
	}
}

void Model::moveTo(Vec3 const _position) {
	position = _position;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}

void Model::scaleTo(Vec3 const _scale) {
	scale = _scale;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}

void Model::rotateX(FLOAT const rox) {
	rotation.x = rox;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}
void Model::rotateY(FLOAT const roy) {
	rotation.y = roy;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}
void Model::rotateZ(FLOAT const roz) {
	rotation.z = roz;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}

void Model::loadModel(const WCHAR* filepath) {
	//
	checkFileExist(filepath);
	//载入器
	Assimp::Importer importer;
	//场景指针，执行三角化操作、翻转纹理坐标
	string sFilepath = ws2s(std::wstring(filepath));
	const aiScene* scene = importer.ReadFile(sFilepath, aiProcess_Triangulate | aiProcess_FlipWindingOrder);
	//错误检测
	if (scene == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
		printf("[Error] Model Loading Error: %s\n\n", importer.GetErrorString());
		return;
	}
	// 记录文件目录以加载相对路径文件(如纹理)
	size_t pos = sFilepath.rfind('/');
	directory = sFilepath.substr(0, pos + 1);
	//
	dLog("[Info] ===== Loading model with total %d meshes: ===== \n", scene->mNumMeshes);
	// 从根节点开始遍历加载模型
	processNode(scene->mRootNode, scene);
	//
	dLog("[Info] ===== Model loading finished. ===== \n\n");
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	dLog("    |-- Load %d Meshes from a node.\n", node->mNumMeshes);
	// 生成当前节点的所有Mesh
	for (UINT i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	// 递归处理子节点
	for (UINT i = 0; i < node->mNumChildren; ++i) {
		this->processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	// 构造Mesh需要的数据
	vector<MeshVertexPNT> vertices;
	vector<UINT> indices;
	vector<shared_ptr<Texture> > textures;
	vector<TextureType> texTypes;
	//
	dLog("        |-- Load %d Vertices from a mesh.\n", mesh->mNumVertices);
	// 处理顶点数据
	for (UINT i = 0; i < mesh->mNumVertices; ++i) {
		MeshVertexPNT vertex;
		Vec3 tmpVec3;
		Vec2 tmpVec2;
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
			tmpVec2.y = 1.0f - mesh->mTextureCoords[0][i].y;
			vertex.texCoord = tmpVec2;
		}
		// 把顶点放进去
		vertices.push_back(vertex);
	}
	// 处理索引数据(由于开启了三角化，所以mNumIndices事实上永远为3)
	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//处理纹理数据
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		loadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE, textures, texTypes);
		loadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_SPECULAR, textures, texTypes);
		//loadMaterialTextures(material, aiTextureType_NORMALS, TEXTURE_NORMAL, textures, texTypes);
		// 不知道为啥 assimp 把map_bump 解析成heightmap
		loadMaterialTextures(material, aiTextureType_HEIGHT, TEXTURE_NORMAL, textures, texTypes);
		loadMaterialTextures(material, aiTextureType_LIGHTMAP, TEXTURE_AMBIENT_OCCLUSION, textures, texTypes);
	}
	// Debug 输出
	dLog("        |-- Process Mesh with:\n");
	dLog("            Normal? : %s.\n", mesh->mNormals ? "Yes" : "No");
	dLog("            Vertices: %d\n", vertices.size());
	dLog("            Indices : %d\n", indices.size());
	dLog("            Textures: %d\n", textures.size());
	// 把生成的网格对象压入成员变量
	meshes.push_back(unique_ptr<Mesh> (new Mesh(vertices, indices, textures, texTypes)));
}

void Model::loadMaterialTextures(aiMaterial* material, aiTextureType typeAssimp, 
	TextureType typeNene, vector<shared_ptr<Texture> > &textures, vector<TextureType> &texTypes) {
	// 一次性读取同类型的纹理
	for (UINT i = 0; i < material->GetTextureCount(typeAssimp); ++i) {
		// 获取纹理的文件名
		aiString texFileName;
		material->GetTexture(typeAssimp, i, &texFileName);
		// 变成纹理的文件路径
		string texFilePath = directory + texFileName.C_Str();
		// 检测之前是否读取过
		auto it = cachedTextures.find(texFilePath);
		if (it == cachedTextures.end()) {
			// 之前没读取过, 生成新的纹理对象
			shared_ptr<Texture> pNewTexture(new Texture(s2ws(texFilePath).c_str()));
			cachedTextures.insert(make_pair(texFilePath, pNewTexture));
			textures.push_back(pNewTexture);
			dLog("            loaded new texture file[type: %d]: %s\n", typeNene, texFilePath.c_str());
		} else {
			// 之前已经读取过，避免重复读取
			textures.push_back(it->second);
			dLog("            loaded cached texture file[type: %d]: %s\n", typeNene, texFilePath.c_str());
		}
		// 写入该纹理的类型
		texTypes.push_back(typeNene);
	}

}

string Model::ws2s(const wstring& s) {
	const size_t slength = s.length();
	const int len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, NULL, 0, NULL, NULL);
	char* buf = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, buf, len, NULL, NULL);
	string r(buf, len);
	delete[] buf;
	return r;
}

wstring Model::s2ws(const string& s) {
	const size_t slength = s.length();
	const int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, buf, len);
	wstring r(buf, len);
	delete[] buf;
	return r;
}