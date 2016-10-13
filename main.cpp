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

//Helper Classes
#include "FPSCounter.h"
#include "Shader.h"
#include "TextureManager.h"
//Toolset
#include "utils.hpp"
#include "tutorials/Benchmark.hpp"
//Tutorial and exercises
#include "tutorials/HelloTriangle.hpp"
#include "tutorials/Shaders.hpp"
#include "tutorials/Textures.hpp"
#include "tutorials/Transformations.hpp"
#include "tutorials/CoordinateSystem.hpp"
#include "tutorials/Camera.hpp"

int main(){
    //Benchmark::test();
    CoordinateSystem::tutorial();
    return 0;
}


