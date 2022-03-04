#include "Projection.h"

#ifndef PERSPECTIVE_PROJECTION
#define PERSPECTIVE_PROJECTION

class LIB_API PerspectiveProjection : public Projection {
public:
	PerspectiveProjection(int width, int heigth, float fov = 45.0f, float near = 1.0f, float far = 100.0f);
	~PerspectiveProjection();

public:
	void update() override;

private:
	float fov;
	float near;
	float far;
};

#endif //PERSPECTIVE_PROJECTION