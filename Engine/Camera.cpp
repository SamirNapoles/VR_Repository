#include "Camera.h"

Camera::Camera(int id, const std::string name, Projection* projection) :
	Node{ id, name }, projection(projection) {}
Camera::~Camera() {};

Projection LIB_API* Camera::getProjection() {
	return projection;
}

glm::mat4 LIB_API Camera::getInverse() {
	return glm::inverse(getFinalMatrix());
}