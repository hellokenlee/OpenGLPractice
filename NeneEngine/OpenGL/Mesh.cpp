/*Copyright reserved by KenLee@2018 ken4000kl@gmail.com*/
#include "Mesh.h"

using namespace std;

Mesh::Mesh(vector<MeshVertex>& _vertices, vector<UInt>& _indices,
           vector<std::shared_ptr<Texture>>& _textures, vector<TextureType>& _texTypes) {
    // 初始化
    textures = _textures;
    texTypes = _texTypes;
    VAO = 0;
    VBO = 0;
    EBO = 0;
    indexNum = _indices.size();
    vertexNum = _vertices.size();
    // 申请显存
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 初始化显存中数据
    glBindVertexArray(VAO);
    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(MeshVertex), &_vertices[0], GL_STATIC_DRAW);
    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(GLuint), &_indices[0], GL_STATIC_DRAW);
    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    // 法向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    //
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if(VAO != 0) glDeleteVertexArrays(1, &VAO);
    if(VBO != 0) glDeleteBuffers(1, &VBO);
    if(indexNum > 0 && EBO != 0) glDeleteBuffers(1, &EBO);
}

void Mesh::bindAllTextures(const Shader* pShader) {
    // 起始纹理编号
    GLuint diffuseNum = 1, specularNum = 1, reflectNum = 1;
    // Uniform名字
    static char uniformName[512];
    // 绑定所有纹理
    for(unsigned int i = 0; i < textures.size(); ++i) {
        // 切换不同的Uniform名字
        switch(texTypes[i]) {
            case TEXTURE_DIFFUSE:
                sprintf(uniformName, "texture_diffuse%d", diffuseNum++);
                break;
            case TEXTURE_SPECULAR:
                sprintf(uniformName, "texture_specular%d", specularNum++);
                break;
            case TEXTURE_REFLECT:
                sprintf(uniformName, "texture_reflection%d", reflectNum++);
                break;
            default:
                ddLog("Ignored %dth texture[type %d].", i, texTypes[i]);
        }
        // 绑定Sampler2D
        glUniform1i(glGetUniformLocation(pShader->programID, uniformName), (UInt)texTypes[i]);
        // 绑定纹理
        textures[i]->use((UInt)texTypes[i]);
    }
}

void Mesh::draw(const Shader* pShader) {
    // 绑定所有纹理
    bindAllTextures(pShader);
    // 绘制
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexNum, GL_UNSIGNED_INT, 0);
}

void Mesh::drawInstanced(const Shader* pShader) {
    //
}
