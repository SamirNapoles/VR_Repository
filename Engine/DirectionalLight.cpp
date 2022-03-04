#include "DirectionalLight.h"

LIB_API DirectionalLight::DirectionalLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular) :
	Light{ id, name, lightNumber, ambient, diffuse, specular } {}

void LIB_API DirectionalLight::render(glm::mat4 cameraInv) {

	glm::vec4 pos = getPosition();
	pos[3] = 0.0f;
	setPosition(pos);

	Light::render(glm::mat4(1.0f));
}

void LIB_API DirectionalLight::setTransform(glm::mat4 transform) {
	glm::vec4 pos = transform[3];
	pos[3] = 0.0f;
	setPosition(pos);

	Node::setTransform(transform);
}