/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

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

