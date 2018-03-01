/*Copyright reserved by KenLee@2018 ken4000kl@gmail.com*/

#include "Shape.h"

Shape::Shape(GLfloat *vertices, GLuint vNum, VertexFormat _vf, GLenum _mode,
             GLuint *indices, GLuint iNum) : Drawable() {
    // 初始化
    VAO = 0;
    VBO = 0;
    EBO = 0;
    TBN_VBO = 0;
    drawMode = _mode;
    vertexNum = vNum;
    indexNum = iNum;
    vf = _vf;
    // 申请显存
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    if(indexNum != 0) {
        // 如果有索引
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }
    switch(vf) {
        case POSITIONS: {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 3 * vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            break;
        }
        case POSITIONS_NORMALS: {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 6 * vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            break;
        }
        case POSITIONS_TEXTURES: {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 5 * vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            break;
        }
        case POSITIONS_NORMALS_TEXTURES: {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 8 * vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            break;
        }
    }
    glBindVertexArray(0);
}

Shape::Shape(Shape &&rhs) : Drawable(std::move(rhs)) {
    // 移动
    VAO = rhs.VAO;
    VBO = rhs.VBO;
    EBO = rhs.EBO;
    TBN_VBO = rhs.TBN_VBO;
    drawMode = rhs.drawMode;
    vertexNum = rhs.vertexNum;
    indexNum = rhs.indexNum;
    vf = rhs.vf;
    // 清空
    rhs.VAO = 0;
    rhs.VBO = 0;
    rhs.EBO = 0;
    rhs.TBN_VBO = 0;
    rhs.vertexNum = 0;
    rhs.indexNum = 0;
}

Shape::~Shape() {
    if(VAO != 0) glDeleteVertexArrays(1, &VAO);
    if(VBO != 0) glDeleteBuffers(1, &VBO);
    if(TBN_VBO != 0) glDeleteBuffers(1, &TBN_VBO);
    if(indexNum > 0 && EBO != 0) glDeleteBuffers(1, &EBO);
}

void Shape::setDrawMode(GLenum newMode) {
    drawMode = newMode;
}

void Shape::draw(const Shader* pShader, const Camera* pCamera) {
    if(pShader != nullptr) {
        pShader->use();
        glUniformMatrix4fv(glGetUniformLocation(pShader->programID, "model"), 1, GL_FALSE, glm::value_ptr(getModelMat()));
        if(pCamera != nullptr) {
            glUniformMatrix4fv(glGetUniformLocation(pShader->programID, "view"), 1, GL_FALSE, pCamera->getViewMatrixVal());
            glUniformMatrix4fv(glGetUniformLocation(pShader->programID, "projection"), 1, GL_FALSE, pCamera->getProjectionMatrixVal());
        }
    }
    glBindVertexArray(VAO);
    if(EBO != 0) {
        glDrawElements(drawMode, indexNum, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(drawMode, 0, vertexNum);
    }
    glBindVertexArray(0);
}

void Shape::drawInstanced(const Shader* pShader, const Camera* pCamera) {
}

void Shape::calcTangents(GLfloat *vertices) {
    if(this->vf != POSITIONS_NORMALS_TEXTURES) {
        // 若无纹理坐标，返回错误
        dLog("Shape::calcTangents() only support POSITIONS_NORMALS_TEXTURES mode\n");
        return;
    }
    if(this->drawMode != GL_TRIANGLES) {
        // 暂时只支持三角形的计算
        dLog("Shape::calcTangents() can not calc tangents from non-triangle-mesh\n");
        return;
    }
    if(this->TBN_VBO != 0) {
        // 检查是否已经计算过了
        dLog("Shape::calcTangents() tangents have already been calced\n");
        return;
    }

    int stride = 8;

    if(EBO == 0) {
        std::vector<glm::vec3> TBN;
        for(unsigned int i = 0; i < vertexNum; i += 3) {
            // 取出对应顶点下标
            int index1 = i + 0;
            int index2 = i + 1;
            int index3 = i + 2;
            // 取出对应顶点位置坐标
            glm::vec3 pos1(vertices[index1 * stride + 0], vertices[index1 * stride + 1], vertices[index1 * stride + 2]);
            glm::vec3 pos2(vertices[index2 * stride + 0], vertices[index2 * stride + 1], vertices[index2 * stride + 2]);
            glm::vec3 pos3(vertices[index3 * stride + 0], vertices[index3 * stride + 1], vertices[index3 * stride + 2]);
            // 取出法线
            glm::vec3 normal1(vertices[index1 * stride + 3], vertices[index1 * stride + 4], vertices[index1 * stride + 5]);
            glm::vec3 normal2(vertices[index2 * stride + 3], vertices[index2 * stride + 4], vertices[index2 * stride + 5]);
            glm::vec3 normal3(vertices[index3 * stride + 3], vertices[index3 * stride + 4], vertices[index3 * stride + 5]);
            // 取出对应顶点纹理坐标
            glm::vec2 uv1, uv2, uv3;
            // 3, 3, 2
            uv1 = glm::vec2(vertices[index1 * stride + 6], vertices[index1 * stride + 7]);
            uv2 = glm::vec2(vertices[index2 * stride + 6], vertices[index2 * stride + 7]);
            uv3 = glm::vec2(vertices[index3 * stride + 6], vertices[index3 * stride + 7]);
            // 计算边和差值
            glm::vec3 edge1 = pos2 - pos1;
            glm::vec3 edge2 = pos3 - pos1;
            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;
            //
            GLfloat f = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV2.x * deltaUV1.y));
            // 计算TBN
            glm::vec3 T, B, N;
            T.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            T.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            T.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            T = glm::normalize(T);
            //
            B.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            B.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            B.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            B = glm::normalize(B);
            // 三个顶点都要插入TBN
            TBN.push_back(T);
            TBN.push_back(B);
            // TBN.push_back(N);
            TBN.push_back(T);
            TBN.push_back(B);
            //  TBN.push_back(N);
            TBN.push_back(T);
            TBN.push_back(B);
            // TBN.push_back(N);

        }
        // 绑定TBN
        // 根据类型判断已经绑定的location
        GLuint attribLocation = 3;
        //
        glGenBuffers(1, &TBN_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBN_VBO);
        glBufferData(GL_ARRAY_BUFFER, TBN.size() * sizeof(glm::vec3), &TBN[0], GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(attribLocation + 0);
        glVertexAttribPointer(attribLocation + 0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)(0 * sizeof(glm::vec3)));
        glEnableVertexAttribArray(attribLocation + 1);
        glVertexAttribPointer(attribLocation + 1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)(1 * sizeof(glm::vec3)));
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    } else {
        dLog("[Error] calcTangents() only support indexed shape!\n");
    }

}
