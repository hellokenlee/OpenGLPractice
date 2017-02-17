/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//FreeImage
#include <FreeImage.h>
//C++ Library
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
//Helper Classes
#include "wmdge/FPSCounter.h"
#include "wmdge/Shader.h"
#include "wmdge/TextureManager.h"
#include "wmdge/Camera.h"
#include "wmdge/CameraController.h"
#include "wmdge/CoordinateAxes.h"
#include "wmdge/Object.h"
#include "wmdge/Texture.h"
#include "wmdge/Mesh.h"
#include "wmdge/utils.hpp"
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
#include "tutorials/GeometryShader.hpp"

int main(){

    ModelLoading::modelTest();
    return 0;
}

