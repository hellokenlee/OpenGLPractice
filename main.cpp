/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//FreeImage
#include <FreeImage.h>
//C++ Library
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <map>
//Helper Classes
#include "wmdge/FPSCounter.h"
#include "wmdge/Shader.h"
#include "wmdge/TextureManager.h"
#include "wmdge/Camera.h"
#include "wmdge/CameraController.h"
#include "wmdge/CoordinateAxes.h"
#include "wmdge/Object.h"
#include "wmdge/Texture.h"
#include "wmdge/Model.h"
#include "wmdge/Geometry.h"
#include "wmdge/utils.hpp"
#include "wmdge/ControlPanel.h"
#include "wmdge/CurveCollection.h"
//Tutorial and exercises
#include "tutorials/Benchmark.hpp"
#include "tutorials/HelloTriangle.hpp"
#include "tutorials/Shaders.hpp"
#include "tutorials/Textures.hpp"
#include "tutorials/Transformations.hpp"
#include "tutorials/CoordinateSystem.hpp"
#include "tutorials/Camera.hpp"
#include "tutorials/Colors.hpp"
#include "tutorials/BasicLighting.hpp"
#include "tutorials/Materials.hpp"
#include "tutorials/LightMaps.hpp"
#include "tutorials/LightCasters.hpp"
#include "tutorials/MultipleLights.hpp"
#include "tutorials/ModelLoading.hpp"
#include "tutorials/DepthTest.hpp"
#include "tutorials/StencilTest.hpp"
#include "tutorials/Blending.hpp"
#include "tutorials/FaceCulling.hpp"
#include "tutorials/Framebuffers.hpp"
#include "tutorials/Mirror.hpp"
#include "tutorials/Cubemaps.hpp"
#include "tutorials/AdvancedData.hpp"
#include "tutorials/AdvancedGLSL.hpp"
#include "tutorials/GeometryShader.hpp"
#include "tutorials/Instancing.hpp"
#include "tutorials/AntiAliasing.hpp"
#include "tutorials/Stella.hpp"
#include "tutorials/BlinnPhong.hpp"
#include "tutorials/GammaCorrection.hpp"
#include "tutorials/NormalMapping.hpp"
#include "tutorials/ShadowMapping.hpp"
#include "tutorials/PointShadows.hpp"
#include "tutorials/ParallaxMapping.hpp"
#include "tutorials/HDR.hpp"
#include "tutorials/Bloom.hpp"
#include "tutorials/PBR.hpp"
#include "tutorials/DeferredShading.hpp"
#include "tutorials/SSAO.hpp"

void test(){
    // 环境初始化
    GLFWwindow *window = initWindow("Yarn_level_Cloth", 800, 600);
    showEnviroment();
    //glfwSwapInterval(0);
    CameraController::bindControl(window);
    CoordinateAxes ca(&CameraController::camera);
    Camera *cam = &CameraController::camera;
    ControlPanel panel(window);
    FPSCounter fc;
    //
    Union *bcc = CurveCollection::genFromBBCFile("textures/Yarn-level Cloth Models/tea_cozy.bcc");
    bcc->isShareModel= true;
    cout<<bcc->objList.size()<<endl;
    //
    Shader whiteShader("shaders/Share/Color.vert", "shaders/Share/Color.frag");
    //
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        panel.draw();

        whiteShader.use();
        glUniformMatrix4fv(glGetUniformLocation(whiteShader.programID, "view"), 1, GL_FALSE, cam->getViewMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(whiteShader.programID, "projection"), 1, GL_FALSE, cam->getProjectionMatrixVal());
        glUniformMatrix4fv(glGetUniformLocation(whiteShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
        bcc->draw(&whiteShader);
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(){
    //SSAO::tutorial();
    test();
    return 0;
}
