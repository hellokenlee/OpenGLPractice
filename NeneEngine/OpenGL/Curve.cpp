/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/

#include "Curve.h"


Shape* Curve::catmullRomSpline(const vector<glm::vec3> &controlPoints, int nPoints) {
    vector<glm::vec3> points = CRChain(controlPoints, nPoints);
    Shape* curve = new Shape(&points[0].x, points.size(), POSITIONS, GL_LINE_STRIP);
    return curve;
}

//
void Curve::CRSpline(vector<glm::vec3> &container, const glm::vec3 &P0, const glm::vec3 &P1, const glm::vec3 &P2, const glm::vec3 &P3, int nPoints) {
    float t0 = 0.0f;
    float t1 = CRTi(t0, P0, P1);
    float t2 = CRTi(t1, P1, P2);
    float t3 = CRTi(t2, P2, P3);

    const float dt = (t2 - t1) / float(nPoints);
    for(float t = t1; t < t2; t += dt) {
        glm::vec3 A1 = ((t1 - t) / (t1 - t0) * P0) + (((t - t0) / (t1 - t0)) * P1);
        glm::vec3 A2 = ((t2 - t) / (t2 - t1) * P1) + (((t - t1) / (t2 - t1)) * P2);
        glm::vec3 A3 = ((t3 - t) / (t3 - t2) * P2) + (((t - t2) / (t3 - t2)) * P3);

        glm::vec3 B1 = ((t2 - t) / (t2 - t0) * A1) + (((t - t0) / (t2 - t0)) * A2);
        glm::vec3 B2 = ((t3 - t) / (t3 - t1) * A2) + (((t - t1) / (t3 - t1)) * A3);

        glm::vec3 C = ((t2 - t) / (t2 - t1) * B1) + (((t - t1) / (t2 - t1)) * B2);

        container.push_back(C);
    }
}

//
float Curve::CRTi(float ti_1, const glm::vec3 &Pi_1, const glm::vec3 &Pi) {
    // 假设 \aplha = 1.0
    float dx = Pi.x - Pi_1.x;
    float dy = Pi.y - Pi_1.y;
    float dz = Pi.z - Pi_1.z;
    return sqrt((dx * dx) + (dy * dy) + (dz * dz)) + ti_1;
}

//
vector<glm::vec3> Curve::CRChain(const vector<glm::vec3> &controlPoints, int nPoints) {
    vector<glm::vec3> res;
    for(unsigned int  i = 0; i < controlPoints.size() - 3; ++i) {
        CRSpline(res, controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], nPoints);
    }
    return res;
}
