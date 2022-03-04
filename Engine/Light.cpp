#include "Light.h"
#include <GL/freeglut.h>

LIB_API Light::Light(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular) :
	Node{ id, name }, lightNumber(lightNumber), ambient(ambient), diffuse(diffuse), specular(specular) {
	glEnable(GL_LIGHTING);
	glEnable(lightNumber);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
}

Light::~Light() {
	Light::nextNumber--;
}

void Light::setPosition() {
	glm::mat4 finalMatrix = getFinalMatrix();
	glm::vec4 finalPosition = finalMatrix[3];
	position = finalPosition;
}

void Light::setPosition(glm::vec4 pos) {
	position = pos;
}

glm::vec4 LIB_API Light::getPosition() {
	return position;
}

void LIB_API Light::setTransform(glm::mat4 transform) {
	Node::setTransform(transform);

	setPosition();
}

int Light::nextNumber = 0;
int LIB_API Light::getNextLightNumber() {
	return GL_LIGHT0 + Light::nextNumber++;
}

int Light::getLightNumber() {
	return lightNumber;
}

float LIB_API Light::getConstantAttenuation() {
	return constantAttenuation;
}
float LIB_API Light::getLinearAttenuation() {
	return linearAttenuation;
}
float LIB_API Light::getQuadraticAttenuation() {
	return quadraticAttenuation;
}
void LIB_API Light::setConstantAttenuation(float constantAttenuation) {
	this->constantAttenuation = constantAttenuation;
}
void LIB_API Light::setLinearAttenuation(float linearAttenuation) {
	this->linearAttenuation = linearAttenuation;
}
void LIB_API Light::setQuadraticAttenuation(float quadraticAttenuation) {
	this->quadraticAttenuation = quadraticAttenuation;
}

void LIB_API Light::render(glm::mat4 cameraInv) {
	glLoadMatrixf(glm::value_ptr(cameraInv * getFinalMatrix()));

	glLightfv(lightNumber, GL_AMBIENT, glm::value_ptr(ambient));
	glLightfv(lightNumber, GL_DIFFUSE, glm::value_ptr(diffuse));
	glLightfv(lightNumber, GL_SPECULAR, glm::value_ptr(specular));

	glLightfv(lightNumber, GL_CONSTANT_ATTENUATION, &constantAttenuation);
	glLightfv(lightNumber, GL_LINEAR_ATTENUATION, &linearAttenuation);
	glLightfv(lightNumber, GL_QUADRATIC_ATTENUATION, &quadraticAttenuation);
	
	glLightfv(lightNumber, GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, position.w)));
}