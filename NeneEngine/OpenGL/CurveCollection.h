/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef CURVE_COLLECTION_H
#define CURVE_COLLECTION_H

#include <vector>

#include "Union.h"
#include "Shape.h"
#include "Curve.h"

// BCC文件头
struct BCCHeader {
    char sign[3];
    unsigned char byteCount;
    char curveType[2];
    char dimensions;
    char upDimension;
    uint64_t curveCount;
    uint64_t totalControlPointCount;
    char fileInfo[40];
};

// 从BCC文件构造模型数据
class CurveCollection {
public:
    static Union* genFromBBCFile(const char* filePath);
private:
    ;
};
#endif // CURVE_COLLECTION_H
