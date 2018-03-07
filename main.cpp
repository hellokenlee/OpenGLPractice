/*Copyright reserved by KenLee@2018 ken4000kl@gmail.com*/

// Graphics Engine
#include "NeneEngine/Sources/OpenGL/Nene.h"
// STD Lib
#include <iostream>
using namespace std;
// Tutorials
#include "Tutorials/HelloTriangle.hpp"
#include "Tutorials/Shaders.hpp"
#include "Tutorials/Textures.hpp"
#include "Tutorials/Transformations.hpp"
#include "Tutorials/CoordinateSystem.hpp"
#include "Tutorials/Camera.hpp"
#include "Tutorials/Colors.hpp"
#include "Tutorials/BasicLighting.hpp"
#include "Tutorials/Materials.hpp"
#include "Tutorials/LightMaps.hpp"
#include "Tutorials/LightCasters.hpp"
#include "Tutorials/MultipleLights.hpp"
#include "Tutorials/ModelLoading.hpp"
#include "Tutorials/DepthTest.hpp"
#include "Tutorials/StencilTest.hpp"
#include "Tutorials/Blending.hpp"
#include "Tutorials/FaceCulling.hpp"
#include "Tutorials/Framebuffers.hpp"
#include "Tutorials/Cubemaps.hpp"
#include "Tutorials/AdvancedData.hpp"
#include "Tutorials/AdvancedGLSL.hpp"
#include "Tutorials/GeometryShader.hpp"
//#include "Tutorials/Instancing.hpp" -> 和现在的引擎不适配
#include "Tutorials/AntiAliasing.hpp"
#include "Tutorials/BlinnPhong.hpp"
#include "Tutorials/GammaCorrection.hpp"
#include "Tutorials/NormalMapping.hpp"
#include "Tutorials/ShadowMapping.hpp"
#include "Tutorials/PointShadows.hpp"
#include "Tutorials/ParallaxMapping.hpp"
#include "Tutorials/HDR.hpp"
#include "Tutorials/Bloom.hpp"
#include "Tutorials/PBR.hpp"
#include "Tutorials/DeferredShading.hpp"
#include "Tutorials/SSAO.hpp"


// Whims
#include "Whims/Mirror.hpp"
#include "Whims/Stella.hpp"
//#include "Whims/TessellationShader.hpp"
//#include "Whims/PNTriangle.hpp"
//#include "Whims/YarnLevelCloth.hpp"
//#include "Whims/VolVisualize.hpp"

// Tests
void runAllTutoials();

// APP Entry
int main(){
    runAllTutoials();
    return 0;
}

void runAllTutoials() {
    /*
    HelloTriangle::tutorial();
    HelloTriangle::exercies1();
    HelloTriangle::exercise2();
    HelloTriangle::exercies3();

    Shaders::tutorial();
    Shaders::exercise1();
    Shaders::exercise2();
    Shaders::exercise3();

    Textures::tutorial();
    Textures::exercise1();
    Textures::exercise2();
    Textures::exercise3();
    Textures::exercise4();

    Transformations::tutorial();
    Transformations::exercise1();
    Transformations::exercise2();

    CoordinateSystem::tutorial();
    Cameras::tutorial();
    Cameras::exercise1();

    Colors::tutorial();
    BasicLighting::tutorial();
    BasicLighting::exercise1();
    BasicLighting::exercise3();
    BasicLighting::exercise4();

    Materials::tutorial();
    Materials::exercise1();

    LightMaps::tutorial();
    LightMaps::exercise2();
    LightMaps::exercise3();
    LightMaps::exercise4();

    LightCasters::tutorialDirectionalLight();
    LightCasters::tutorialPointLight();
    LightCasters::tutorialSpotLight();

    MultipleLights::tutorial();

    ModelLoading::tutorial();
    ModelLoading::exercise1();

    DepthTest::tutorial();

    StencilTest::tutorial();
    StencilTest::exercise1();

    Blending::tutorial();
    Blending::exercise1();
    FaceCulling::tutorial();
    FaceCulling::exercise1();

    Framebuffers::tutorial();
    Framebuffers::exercise1();

    Cubemapes::tutorial();
    Cubemapes::exercise1();
    AdvancedData::tutorial();
    AdvancedGLSL::tutorial();
    AdvancedGLSL::exercise();

    GeometyShader::tutorial();
    GeometyShader::exercise1();
    GeometyShader::exercise2();

    AntiAliasing::tutorial();
    AntiAliasing::exercise();

    BlinnPhong::tutorial();

    GammaCorrection::tutorial();

    NormalMapping::tutorial();

    ShadowMapping::tutorial();
    ShadowMapping::exercise1();
    ShadowMapping::exercise2();

    PointShadows::tutorial();

    ParallaxMapping::tutorial();

    HDR::tutorial();

    Bloom::tutorial();

    PBR::tutorial();
    PBR::exercise2();

    DeferredShading::tutorial();
    DeferredShading::exercise1();
    SSAO::tutorial();
    //*/

    Mirror::_main();
}
