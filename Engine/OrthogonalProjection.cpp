#include "OrthogonalProjection.h"

LIB_API OrthogonalProjection::OrthogonalProjection(int width, int heigth) :
	OrthogonalProjection(width, heigth, 0.0f, (float)width, (float)heigth, 0.0f, -1.0f, 1.0f) {};

LIB_API OrthogonalProjection::OrthogonalProjection(int width, int heigth, float left, float right, float bottom, float top, float zNear, float zFar) :
	Projection{ width, heigth }, left(left), right(right), bottom(bottom), top(top), zNear(zNear), zFar(zFar) {

	update();
};
LIB_API OrthogonalProjection::~OrthogonalProjection() {}

void OrthogonalProjection::update() {
	glm::mat4 p = glm::ortho(
		left,
		right,
		bottom,
		top,
		zNear,
		zFar
	);

	this->setProjection(p);
}