#include "PointLight.h"
#include <GL/freeglut.h>

LIB_API PointLight::PointLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, 
	const glm::vec4 diffuse, const glm::vec4 specular, const float cutOff) :
	Light{ id, name, lightNumber, ambient, diffuse, specular}, cutOff(cutOff) {};

void LIB_API PointLight::render(glm::mat4 cameraInv) {
	glLightfv(getLightNumber(), GL_SPOT_CUTOFF, &cutOff);

	Light::render(cameraInv);
}