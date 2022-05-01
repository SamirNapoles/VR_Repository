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

glm::vec3 LIB_API Camera::getFrontDirection() {
	glm::mat4 t = getFinalMatrix();
	return glm::vec3(t[2][0], t[2][1], t[2][2]) * -1.0f;
}

glm::vec3 LIB_API Camera::getRightDirection() {
	glm::mat4 t = getFinalMatrix();
	return glm::vec3(t[0][0], t[0][1], t[0][2]);
}