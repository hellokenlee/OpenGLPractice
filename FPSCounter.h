/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H


#include <GLFW/glfw3.h>
#include <stdio.h>

class FPSCounter{
public:
	//构造函数
	FPSCounter();
	//在主循环调用，控制台输出平均绘制速度
	void update();
private:
	double lastTime,currentTime;
	int frameCounter;
};


#endif	//	FPS_COUNTER_H
