#include "Projection.h"

#ifndef ORTHOGONAL_PROJECTION
#define ORTHOGONAL_PROJECTION

class LIB_API OrthogonalProjection : public Projection {
public:
	OrthogonalProjection(int width, int heigth);
	OrthogonalProjection(int width, int heigth, float left, float right, float bottom, float top, float zNear, float zFar);
	~OrthogonalProjection();

public:
	void update() override;

private:
	float left;
	float right;
	float bottom;
	float top;
	float zNear;
	float zFar;
};

#endif //ORTHOGONAL_PROJECTION