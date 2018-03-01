/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef VOXEL_OBJECT_H
#define VOXEL_OBJECT_H

#include "Shape.h"
#include "Union.h"
#include <vector>

//
//	体元组成的物体： 使用实例化渲染
//

#define VOXEL_SIZE 0.01

class VoxelShape{
public:
    VoxelShape(std::vector<float> &data, const int numX, const int numY, const int numZ);
    //设置Shader
    void setShader(Shader* _shader);
    //设置相机
    void setCamera(Camera* _camera);
    void draw();
public:
    std::vector<glm::mat4> modelMats;
    Shape* cube;
    Shader* shader;
    Camera* camera;

private:
    VoxelShape();
    VoxelShape& operator=(const VoxelShape);
};

#endif // VOXEL_OBJECT_H


