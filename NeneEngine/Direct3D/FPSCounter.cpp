/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "FPSCounter.h"

//init
FPSCounter::FPSCounter() {
    lastTime =  currentTime = time(nullptr);
    frameCounter = 0;
    frameTime = -1.0;
}

double FPSCounter::getFrameTime(){
    return frameTime;
}

void FPSCounter::print(){
    if(update()){
        printf("%lf ms/frame\n", frameTime);
    }
}

//called in main loop
bool FPSCounter::update() {
    currentTime = time(nullptr);
    ++frameCounter;
    if(currentTime - lastTime >= 1) {
        frameTime = 1000.0 / double(frameCounter);
        frameCounter = 0;
        lastTime += 1;
        return true;
    }
    return false;
}
