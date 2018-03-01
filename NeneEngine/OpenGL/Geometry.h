/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "Shape.h"
#include <cmath>
#include <vector>
#include <unordered_map>
#include <memory>

//
//	几何类（Shape 类的工厂类）
//
class Geometry {
public:
    // 创建一个球体(UV法)
    static std::unique_ptr<Shape> createUVSphere();
    // 创建一个球体(20面体细分法)
    static std::unique_ptr<Shape> createIcoSphere(const int level);
    // 创建一个立方体
    static std::unique_ptr<Shape> createCube();
    // 创建一个长方形(默认在XoZ平面上)
    static std::unique_ptr<Shape> createQuad(const GLfloat width = 1.0f, const GLfloat height = 1.0f);
private:
    // createIcoSphere 调用
    static int createIcoSphere_GetMiddle(std::unordered_map<long long int, GLuint> &vertexHash,
                         std::vector<glm::vec3> &vertices, int index1, int index2);

};

#endif
