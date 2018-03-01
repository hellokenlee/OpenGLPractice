/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#include "CurveCollection.h"
#include <iostream>

using namespace std;

// 从BCC文件生成曲线集合模型
Union* CurveCollection::genFromBBCFile(const char* filePath) {
    // 读取文件头
    BCCHeader header;
    FILE *file = fopen(filePath, "rb");
    fread(&header, sizeof(header), 1, file);
    // 验证文件头是否出错
    if(header.sign[0] != 'B' || header.sign[1] != 'C' || header.sign[2] != 'C') {
        printf("CurveCollection: BCC File Error in Sign!\n");
        return nullptr;
    }
    if(header.byteCount != 0x44) {
        printf("CurveCollection: BCC File Error in byte!\n");
        return nullptr;
    }
    if(header.curveType[0] != 'C' || header.curveType[1] != '0') {
        printf("CurveCollection: BCC File Error in Curve Type!\n");
        return nullptr;
    }
    if(header.dimensions != 3) {
        printf("CurveCollection: BCC File Error in Dimensions!\n");
        return nullptr;
    }

    cout << "[BCC] Total Control Points Num: " << header.totalControlPointCount << endl;
    cout << "[BCC] Curve Num: %llu\n" << header.curveCount <<endl;

    Union* res = new Union();

    for(unsigned int i = 0; i < header.curveCount; ++i) {
        // 读取该组控制点的数量
        int cpCount;
        fread(&cpCount, sizeof(int), 1, file);
        // 取绝对值
        ///！ 这里忽略了负数成环的情况
        cpCount = abs(cpCount);
        //
        vector<glm::vec3> controlPoints(cpCount);
        // [x1, y1, z1, x2, y2, z2, ...]
        fread(&controlPoints[0].x, sizeof(float), cpCount * 3, file);
        //
        //Shape *curve = new Shape(&controlPoints[0].x, cpCount, POSITIONS, GL_POINTS);
        Shape* curve = Curve::catmullRomSpline(controlPoints, 10);
        res->addShape(curve);
        //
        printf("[BCC] %d th Control Points Num: %d\n", i, cpCount);
    }
    return res;
}
