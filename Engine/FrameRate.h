#include "LibDef.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#ifndef FRAMERATE
#define FRAMERATE

class LIB_API FrameRate {
public:
	FrameRate();
	~FrameRate();

public:
	int getFps();
	void calculateFrameRate();

private:
	int fps = 0;
	int fpsTmp = 0;
	time_t lastTime = 0.0f;
	time_t currentTime = 0.0f;
};

#endif //FRAMERATE