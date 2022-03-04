#include "PerspectiveProjection.h"

LIB_API PerspectiveProjection::PerspectiveProjection(int width, int heigth, float fov, float near, float far) :
	Projection{ width, heigth }, fov(fov), near(near), far(far) {

	update();
};
LIB_API PerspectiveProjection::~PerspectiveProjection() {}

void PerspectiveProjection::update() {
	glm::mat4 p = glm::perspective(
		glm::radians(fov),
		getWidth() / (float)getHeigth(),
		near,
		far
	);

	this->setProjection(p);
}