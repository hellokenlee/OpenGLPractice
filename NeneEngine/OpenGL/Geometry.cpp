/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/

#include "Geometry.h"

using namespace glm;
using namespace std;

unique_ptr<Shape> Geometry::createIcoSphere(const int level){
    // 3个互相垂直的长方形
    std::vector<glm::vec3> vertices;// -> VBO
    GLfloat t = (1.0 + sqrt(5.0)) / 2.0;
    GLfloat len = sqrt(1.0 + (t * t));
    t /= len;
    GLfloat s = 1.0 / len;
    // XoY平面上
    vertices.push_back(glm::vec3(-s,  t, 0.0f));
    vertices.push_back(glm::vec3( s,  t, 0.0f));
    vertices.push_back(glm::vec3(-s, -t, 0.0f));
    vertices.push_back(glm::vec3( s, -t, 0.0f));
    // YoZ平面上
    vertices.push_back(glm::vec3(0.0f, -s,  t));
    vertices.push_back(glm::vec3(0.0f,  s,  t));
    vertices.push_back(glm::vec3(0.0f, -s, -t));
    vertices.push_back(glm::vec3(0.0f,  s, -t));
    // XoZ平面上
    vertices.push_back(glm::vec3( t, 0.0f, -s));
    vertices.push_back(glm::vec3( t, 0.0f,  s));
    vertices.push_back(glm::vec3(-t, 0.0f, -s));
    vertices.push_back(glm::vec3(-t, 0.0f,  s));
    // 根据上面的12个顶点构造20面体
    std::vector<glm::uvec3> faces;// -> EBO
    // 0顶点相邻的5个三角面
    faces.push_back(glm::uvec3(0, 11,  5));
    faces.push_back(glm::uvec3(0,  5,  1));
    faces.push_back(glm::uvec3(0,  1,  7));
    faces.push_back(glm::uvec3(0,  7, 10));
    faces.push_back(glm::uvec3(0, 10, 11));
    //
    faces.push_back(glm::uvec3( 1,  5, 9));
    faces.push_back(glm::uvec3( 5, 11, 4));
    faces.push_back(glm::uvec3(11, 10, 2));
    faces.push_back(glm::uvec3(10,  7, 6));
    faces.push_back(glm::uvec3( 7,  1, 8));
    // 3顶点附近的5个三角面
    faces.push_back(glm::uvec3(3, 9, 4));
    faces.push_back(glm::uvec3(3, 4, 2));
    faces.push_back(glm::uvec3(3, 2, 6));
    faces.push_back(glm::uvec3(3, 6, 8));
    faces.push_back(glm::uvec3(3, 8, 9));
    //
    faces.push_back(glm::uvec3(4, 9,  5));
    faces.push_back(glm::uvec3(2, 4, 11));
    faces.push_back(glm::uvec3(6, 2, 10));
    faces.push_back(glm::uvec3(8, 6,  7));
    faces.push_back(glm::uvec3(9, 8,  1));

    // 细分
    std::unordered_map<long long int, GLuint> verticesHash;
    for(int i = 0; i < level; ++i){
        std::vector<glm::uvec3> faces2;
        for(unsigned int f = 0; f < faces.size(); ++f){
            // 原三角形的三个顶点编号
            int p1 = faces[f].x;
            int p2 = faces[f].y;
            int p3 = faces[f].z;
            int a = createIcoSphere_GetMiddle(verticesHash, vertices, p1, p2);
            int b = createIcoSphere_GetMiddle(verticesHash, vertices, p2, p3);
            int c = createIcoSphere_GetMiddle(verticesHash, vertices, p3, p1);
            // abc为新的内接三角形： 一个大三角形P1P2P3 -> 四个小三角形
            faces2.push_back(glm::uvec3(p1, a, c));
            faces2.push_back(glm::uvec3(p2, b, a));
            faces2.push_back(glm::uvec3(p3, c, b));
            faces2.push_back(glm::uvec3( a, b, c));
        }
        faces = faces2;// 细分后的面取代原来的面
    }
    //
    dLog("[Info] Create icosphere with level %d :\n", level);
	dLog("    Vertices Num: %d \n", vertices.size());
	dLog("    Faces Num: %d \n", faces.size());
    // 计算纹理坐标
    std::vector<GLfloat> verticesPNT;
    for(unsigned int f = 0; f < faces.size(); ++f){
        GLfloat x[3], y[3], z[3];
        GLfloat u[3], v[3];
        bool isPole = false;
        int poleVertex = -1;
        for(int i = 0; i < 3; ++i){
            int index =faces[f][i];
            x[i] = vertices[index].x;
            y[i] = vertices[index].y;
            z[i] = vertices[index].z;
            // 计算法纹理坐标
            u[i] = 0.5f + (atan2(-z[i], -x[i]) / M_PI / 2.0f);
            v[i] = 0.5f - (asin(-y[i]) / M_PI);
            //
            if(x[i] == 0.0 && z[i] == 0.0 && (y[i] == 1.0 || y[i] == -1.0)){
                poleVertex = i;
                isPole = true;
            }
        }
        //修复球体边上的锯齿状纹理错误
        glm::vec3 texNormal;
        texNormal = glm::cross(glm::vec3(u[2], v[2], 0.0f) - glm::vec3(u[0], v[0], 0.0f),
                                         glm::vec3(u[1], v[1], 0.0f) - glm::vec3(u[0], v[0], 0.0f));
        if(texNormal.z < 0){
            for(int i = 0; i < 3; ++i){
                if(u[i] < 0.5)
                    u[i] += 1.0;
            }
        }
        if(isPole){
            u[poleVertex] = (u[(poleVertex + 1) % 3] + u[(poleVertex + 2) % 3]) / 2;
        }
        for(int i = 0; i < 3; ++i){
            // POSITION
            verticesPNT.push_back(x[i]);
            verticesPNT.push_back(y[i]);
            verticesPNT.push_back(z[i]);
            // NORMAL 球体的点的法向量等于点的坐标(因为球心为原点)
            verticesPNT.push_back(x[i]);
            verticesPNT.push_back(y[i]);
            verticesPNT.push_back(z[i]);
            // TEXTURES
            verticesPNT.push_back(u[i]);
            verticesPNT.push_back(v[i]);
        }
    }
    dLog("    Total Render vertices number(without indexing): %d \n\n", verticesPNT.size() / 8);
    Shape *res = new Shape(&verticesPNT[0], verticesPNT.size() / 8, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    // 返回
    return unique_ptr<Shape>(res);
}

unique_ptr<Shape> Geometry::createCube() {
    const static GLfloat cubeVertices[6 * 6 * 8] = {
        // Position x 3 // Normal x 3 // TextureCoord x 2
        // 后面
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // 前面
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // 左面
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // 右面
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
        // 下面
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // 上面
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         0.5f,  0.5f , 0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
    };
    return unique_ptr<Shape>(new Shape((GLfloat*)cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES));
}

int Geometry::createIcoSphere_GetMiddle(std::unordered_map<long long int, GLuint> &vertexHash,
                                        std::vector<glm::vec3> &vertices, int index1, int index2){

    if(index1 > index2){
        // 保证index1 < index2
        std::swap(index1, index2);
    }
    assert(sizeof(int) == 4 && sizeof(long long int) == 8);
    long long int key = ((long long int)index1 << 32) + index2;
    if(vertexHash.find(key) != vertexHash.end()){
        // 如果已经计算过了该个点
        return vertexHash[key];
    }
    // 如果没有计算过这个点，新生成一个点
    glm::vec3 newVertex((vertices[index1].x + vertices[index2].x) / 2,
                        (vertices[index1].y + vertices[index2].y) / 2,
                        (vertices[index1].z + vertices[index2].z) / 2);
    GLfloat len = glm::distance(newVertex, glm::vec3(0, 0, 0));
    // 单位化处理
    newVertex.x /= len;
    newVertex.y /= len;
    newVertex.z /= len;
    // 把新的顶点放入顶点集
    int res = vertices.size();
    vertices.push_back(newVertex);
    vertexHash[key] = res;
    return res;
};


