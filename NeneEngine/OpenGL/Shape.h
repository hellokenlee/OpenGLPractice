/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <memory>

#include "Shader.h"
#include "Camera.h"
#include "Drawable.h"

//
//	物体类(多个三维顶点的集合)
//

class Shape : public Drawable {
public:
    // 构造函数
    Shape(GLfloat *vertices, GLuint vNum, VertexFormat vf, GLenum mode,
           GLuint *indices = nullptr, GLuint iNum = 0);
    Shape(Shape &&rhs);
    // 析构函数
    virtual ~Shape();
    // 绘制函数
    virtual void draw(const Shader* pShader = nullptr, const Camera* pCamera = nullptr);
    virtual void drawInstanced(const Shader* pShader = nullptr, const Camera* pCamera = nullptr);
    // 根据纹理贴图中的法线方向和顶点的纹理坐标计算切线和副切线
    void calcTangents(GLfloat *vertices);
    // 复制一个相同形状的物体使用同一块显存
    Shape clone();
    // 设置绘制模式
    void setDrawMode(GLenum newMode);
public:
    // 顶点缓冲，
    GLuint VAO, VBO, EBO;
    // TBN矩阵的VBO
    GLuint TBN_VBO;
    // 顶点数
    GLuint vertexNum;
    // 下标数
    GLuint indexNum;
private:
    // 禁止拷贝
    Shape();
    Shape(const Shape& rhs);
    Shape& operator=(const Shape& rhs);
    // 绘制模式 如: GL_LINES
    GLenum	drawMode;
    // 顶点格式
    VertexFormat vf;
};

#endif	//	SHAPE_H
