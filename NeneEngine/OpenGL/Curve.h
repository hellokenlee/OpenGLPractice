/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef CURVE_H
#define CURVE_H

#include <vector>

#include "Shape.h"

using namespace std;

//
//	曲线类(Shape 工厂类)
//
class Curve {
public:
// 构造一个CRS曲线
static Shape* catmullRomSpline(const vector<glm::vec3> &controlPoints, int nPoints = 100);

//private:
///////////// CatmullRomSpline Functions ///////////////////////
// 根据 t_i = [{\sqrt{(x_i-x_{i-1})^2+(y_i-y_{i-1})^2}}]^\alpha + t_{i-1} 计算ti
static float CRTi(float ti_1, const glm::vec3 &Pi_1, const glm::vec3 &Pi);
// 根据四个控制点插值出[P1, P2]间的曲线点
static void CRSpline(vector<glm::vec3> &container, const glm::vec3 &P0, const glm::vec3 &P1, const glm::vec3 &P2, const glm::vec3 &P3, int nPoints);
// 根据控制点P0, P1, P2, P3 ... PN 计算出经过[P1, PN-1]的CRS曲线
static vector<glm::vec3> CRChain(const vector<glm::vec3> &controlPoints, int nPoints);

///////////// BazierCurve Functions ///////////////////////
};

#endif // CURVE_H
