#include "PerspectiveProjection.h"

LIB_API PerspectiveProjection::PerspectiveProjection(int width, int heigth, float fov, float near, float far) :
	Projection{ width, heigth }, p_fov{ fov }, p_near{ near }, p_far{ far } {

	update();
};
LIB_API PerspectiveProjection::~PerspectiveProjection() {}

void PerspectiveProjection::update() {
	glm::mat4 p = glm::perspective(
		glm::radians(p_fov),
		getWidth() / (float)getHeigth(),
		p_near,
		p_far
	);

	this->setProjection(p);
}