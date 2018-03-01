/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef UNION_H
#define UNION_H

#include "Shape.h"

//
//	联合类(多个Shape的联合体)
//

class Union{
public:
    // 构造函数
    Union();
    // 设置Model矩阵
    void setModel(glm::mat4);
    // 往联合中添加一个Shape
    void addShape(Shape* obj);
    // 绘制
    void draw();
    // 设置着色器
    void setShader(Shader* _sha);
    // 设置相机
    void setCamera(Camera* _cam);
public:
    Shader* shader;
    Camera* camera;
    bool isShareModel;
    std::vector<Shape*> objList;
    std::vector<glm::mat4> objModel;
    glm::mat4 unionModel;
};

#endif
