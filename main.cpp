// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// FreeImage
#include <FreeImage.h>
// C++ Library
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <array>
#include <map>
// WMD Engine
#define CHECK_GL_ERROR
#include "wmdge/utils.hpp"
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
#include "wmdge/ControlPanel.h"
#include "wmdge/CurveCollection.h"
#include "wmdge/Curve.h"
// Tutorial
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
#include "tutorials/Cubemaps.hpp"
#include "tutorials/AdvancedData.hpp"
#include "tutorials/AdvancedGLSL.hpp"
#include "tutorials/GeometryShader.hpp"
#include "tutorials/Instancing.hpp"
#include "tutorials/AntiAliasing.hpp"
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
// Whims
#include "tutorials/Benchmark.hpp"
#include "tutorials/Mirror.hpp"
#include "tutorials/Stella.hpp"
#include "tutorials/YarnLevelCloth.hpp"
#include "tutorials/TessellationShader.hpp"


int main(){
    //YarnLevelCloth::_main();
    TessellationShader::_main();
    return 0;
}

