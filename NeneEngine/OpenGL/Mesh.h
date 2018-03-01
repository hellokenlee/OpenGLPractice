/*Copyright reserved by KenLee@2018 ken4000kl@gmail.com*/
#ifndef MESH_H
#define MESH_H

#include "Types.h"
#include "Texture.h"
#include "Shader.h"

#include <vector>
#include <memory>

//顶点数据结构
struct MeshVertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
};

//
//	网格类
//

class Mesh {
public:
    // 构造函数
    Mesh(std::vector<MeshVertex>& vertices, std::vector<UInt>& indices,
         std::vector<std::shared_ptr<Texture>> &textures, std::vector<TextureType> &texTypes);
    ~Mesh();
    // 绘制函数
    void draw(const Shader* pShader);
    void drawInstanced(const Shader* pShader);
    //
    inline GLuint getVAO() {
        return VAO;
    }
private:
    // 禁止拷贝
    Mesh();
    Mesh(const Mesh& rhs);
    Mesh& operator=(const Mesh& rhs);
    //
    void bindAllTextures(const Shader* pShader);
    // 显存数据索引
    GLuint VAO, VBO, EBO;
    GLuint indexNum, vertexNum;
    // 纹理数组
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<TextureType> texTypes;
};

#endif	//	MESH_H
