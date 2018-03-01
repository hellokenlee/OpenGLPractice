/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include <stdio.h>
#include <time.h>

class FPSCounter {
public:
    // 构造函数
    FPSCounter();
    // 更新帧率
    bool update();
    // 获取帧率
    double getFrameTime();
    // 在控制台输出帧率
    void print();
private:
    time_t lastTime, currentTime;
    double frameTime;
    int frameCounter;
};


#endif	//	FPS_COUNTER_H
