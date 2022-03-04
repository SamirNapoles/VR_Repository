#include "FrameRate.h"
#include <GL/freeglut.h>

LIB_API FrameRate::FrameRate() : lastTime(time(nullptr)) {};
LIB_API FrameRate::~FrameRate() {};

int LIB_API FrameRate::getFps() {
	return fps;
}

void LIB_API FrameRate::calculateFrameRate() {
	currentTime = time(nullptr);
	fpsTmp++;
	if (currentTime - lastTime > 1.0f) {
		lastTime = currentTime;
		fps = fpsTmp;
		fpsTmp = 0;
	}
}
