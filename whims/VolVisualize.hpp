/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef VOL_VISUALIZE_HPP
#define VOL_VISUALIZE_HPP

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace VolVisualize {

class MitsubaVol {
private:
    struct VolHeader {
        //Bytes 1-3
        char v, o, l;
        //Bytes 4
        char version;
        //Bytes 5-8
        int encoding;
        //Bytes 9-20
        int numX, numY, numZ;
        //Bytes 21-24
        int channels;
        //Bytes 25-48
        float xmin, ymin, zmin, xmax, ymax, zmax;
    };
public:
    string fileName;
    VolHeader header;
    vector<float> data;
public:
    bool checkHeader() {
        return (header.v == 'v' && header.o == 'o' && header.l == 'l');
    }
    bool load(const char* filePath) {
        FILE *f = fopen(filePath, "r");
        if(f != nullptr) {
            fileName = string(filePath);
            fread((void*)(&header), 1, 48, f);
            //if(checkHeader()) {
                data.resize(header.numX * header.numY * header.numZ);
                for(int i = 0; i < header.numZ; ++i ) {
                    fread(&data[i * header.numX * header.numY], sizeof(float), header.numX * header.numY, f);
                }
                fclose(f);
                return true;
            //}
            data.resize(header.numX * header.numY * header.numZ);
            for(int i = 0; i < header.numZ; ++i ) {
                fread(&data[i * header.numX * header.numY], sizeof(float), header.numX * header.numY, f);
            }
            fclose(f);
            return true;
        }
        printf("[Error] MitsubaVol: Failed to read %s .\n", filePath);
        return false;
    }
    float retrieve(int x, int y, int z) {
#ifdef DEBUG
        assert(x >= 0 && x < header.numX);
		assert(y >= 0 && y < header.numY);
		assert(z >= 0 && z < header.numZ);
#endif // DEBUG
        int id = (z * header.numY + y) * header.numX + x;

        return data[id];
    }
    void showInfo() {
        printf("[INFO]: Vol FIle: %s\n", fileName.c_str());
        printf("    ID  : %c%c%c\n", header.v, header.o, header.l);
        printf("    Ver : %d\n", header.version);
        printf("    Enc : %d\n", header.encoding);
        printf("    Num : (%d, %d, %d)\n", header.numX, header.numY, header.numZ);
        printf("    Chan: %d\n", header.channels);
        printf("    AABB: (%f, %f, %f, %f, %f, %f)\n\n", header.xmin, header.ymin, header.zmin, header.xmax, header.ymax, header.zmax);
    }
};

const glm::vec3 lightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 2.0f, 2.0f),
    glm::vec3(0.0f, 2.0f, 4.0f),
    glm::vec3(0.0f, 2.0f, 6.0f),
    glm::vec3(0.0f, 2.0f, 8.0f),
};

const glm::vec3 lightAmbient = glm::vec3(0.05f);
const glm::vec3 lightDiffuse = glm::vec3(0.01f);
const glm::vec3 lightSpecular = glm::vec3(0.5f);

void _main() {
    // 初始化
    GLFWwindow *window = initWindow("TessellationShader", 800, 600, 3, 3);
    showEnviroment();
    CameraController::bindControl(window);
    Camera *cam = CameraController::getCamera();
    // 一些设置
    glEnable(GL_DEPTH_TEST);
    CoordinateAxes ca(cam);
    ControlPanel panel(window);
    // 简单颜色着色器
    //Shader shader("Resources/Shaders/Share/Color.vert", "Resources/Shaders/Share/Color.frag");
    Shader shader("Resources/Shaders/Share/VoxelShape.vert", "Resources/Shaders/Share/VoxelShape.frag");
    //
    MitsubaVol mVol;
    mVol.load("Resources/Textures/Yarns/yarn_3_od.vol");
    //mVol.load("/home/kenlee/Workspace/Mitsuba/ClothCTImage/velvetA/smallA_full_part0_od_sub.vol");
    mVol.showInfo();
    /*
    vector<glm::vec3> vertices;
    const float STEP = 0.1;
    for(int x = 0; x < mVol.header.numX; ++x) {
        for(int y = 0; y < mVol.header.numY; ++y) {
            for(int z = 0; z < mVol.header.numZ; ++z) {
                GLfloat alpha = mVol.retrieve(x, y, z);
                if(alpha > 0 ){
                    vertices.push_back(glm::vec3(x * STEP, y * STEP, z * STEP));
                    vertices.push_back(glm::vec3(alpha, alpha, alpha));
                }
            }
        }
    }
    Shape* cube = new Shape(&vertices[0].x, vertices.size() / 2, POSITIONS_COLORS, GL_POINTS);
    //*/
    VoxelShape* cube = new VoxelShape(mVol.data, mVol.header.numX, mVol.header.numY, mVol.header.numZ);
    cube->setShader(&shader);
    cube->setCamera(cam);
    // 写入光源位置
    shader.use();
    for(int i = 0; i < 5; ++i) {
        char uniformName[128] = {0};
        sprintf(uniformName, "lights_VS_in[%d].position", i);
        cout<<uniformName<<endl;
        glUniform3f(glGetUniformLocation(shader.programID, uniformName), lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
        sprintf(uniformName, "lights_VS_in[%d].ambient", i);
        cout<<uniformName<<endl;
        glUniform3f(glGetUniformLocation(shader.programID, uniformName), lightAmbient.r, lightAmbient.g, lightAmbient.b);
        sprintf(uniformName, "lights_VS_in[%d].diffuse", i);
        cout<<uniformName<<endl;
        glUniform3f(glGetUniformLocation(shader.programID, uniformName), lightDiffuse.r, lightDiffuse.g, lightDiffuse.b);
        sprintf(uniformName, "lights_VS_in[%d].specular", i);
        cout<<uniformName<<endl;
        glUniform3f(glGetUniformLocation(shader.programID, uniformName), lightSpecular.r, lightSpecular.g, lightSpecular.b);
    }
    // 主循环
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ca.draw();

        cube->draw();

        panel.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
};
#endif // VOL_VISUALIZE_HPP
