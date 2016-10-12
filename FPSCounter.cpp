/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/

#include "FPSCounter.h"
//init
FPSCounter::FPSCounter(){
    lastTime=currentTime=glfwGetTime();
    frameCounter=0;
}
//called in main loop
void FPSCounter::update(){
    currentTime=glfwGetTime();
    ++frameCounter;
    if(currentTime-lastTime>=1.0){
        printf("%f ms/frame\n",1000.0/double(frameCounter));
        frameCounter=0;
        lastTime+=1.0;
    }
}
