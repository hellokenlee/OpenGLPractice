/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/

#include "VoxelObject.h"
#include "Geometry.h"

#define DEBUG

VoxelShape::VoxelShape(std::vector<float> &data, const int numX, const int numY, const int numZ) {
    // 初始化
    camera = nullptr;
    shader = nullptr;
    cube = Geometry::cube();
    modelMats.clear();
    // 生成Model Instancing Array
    for(int x = 0; x < numX; ++x) {
        for(int y = 0; y < numY; ++y) {
            for(int z = 0; z < numZ; ++z) {
                // 获取该xyz对应的data的下标
                int id = (z * numY + y) * numX + x;
                float density = data[id];
                // 如果该Voxel有数据，建立其Model矩阵
                if(density > 0) {
                    glm::mat4 modelNow = glm::mat4();
                    // 计算偏移， 把左下角的点放在原点。
                    modelNow = glm::translate(modelNow, glm::vec3(VOXEL_SIZE * (x + 0.5), VOXEL_SIZE * (y + 0.5), VOXEL_SIZE * (z + 0.5)));
                    // 缩放
                    modelNow = glm::scale(modelNow, glm::vec3(VOXEL_SIZE, VOXEL_SIZE, VOXEL_SIZE));
                    modelMats.push_back(modelNow);
                }
            }
        }
    }
    printf("[Info] Voxel Num: %d\n", modelMats.size());
    // 把Model数组扔到VBO
    GLuint IA_VBO;
    glGenBuffers(1, &IA_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, IA_VBO);
    glBufferData(GL_ARRAY_BUFFER, modelMats.size() * sizeof(glm::mat4), &modelMats[0], GL_STATIC_DRAW);
    // 写入顶点属性指针(Vertex Attrib Pointer)
    glBindVertexArray(cube->VAO);
        // 绑定实例数组， 一个mat4 = 4个vec4
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(0 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(1 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VoxelShape::setCamera(Camera* _camera) {
    camera = _camera;
}

void VoxelShape::setShader(Shader* _shader) {
    shader = _shader;
}

void VoxelShape::draw() {
    if(shader != nullptr && camera != nullptr) {
        shader->use();
        glUniformMatrix4fv(glGetUniformLocation(shader->programID, "view"), 1, GL_FALSE, camera->getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(shader->programID, "projection"), 1, GL_FALSE, camera->getProjectionMatrixVal());
    }
    glBindVertexArray(cube->VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, cube->vertexNum, modelMats.size());
    glBindVertexArray(0);
}

