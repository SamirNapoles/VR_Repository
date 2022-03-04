#include "SpotLight.h"
#include <GL/freeglut.h>

LIB_API SpotLight::SpotLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, 
	const glm::vec4 diffuse, const glm::vec4 specular, const float cutOff, const glm::vec3 direction) :
	PointLight{ id, name, lightNumber, ambient, diffuse, specular, cutOff }, direction(direction) {};

void LIB_API SpotLight::render(glm::mat4 cameraInv) {
	glLightfv(getLightNumber(), GL_SPOT_DIRECTION, glm::value_ptr(glm::vec4(direction, 1.0f)));

	PointLight::render(cameraInv);
}