#include "PointLight.h"
#include <GL/freeglut.h>

#include "Program.h"

LIB_API PointLight::PointLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, 
	const glm::vec4 diffuse, const glm::vec4 specular, const float cutOff) :
	Light{ id, name, lightNumber, ambient, diffuse, specular}, cutOff(cutOff) {};

void LIB_API PointLight::render(glm::mat4 finalMatrix) {
	Program* activeProgram = Program::getActiveProgram();
	std::map<std::string, int> uniforms = activeProgram->getUniforms();
	activeProgram->setVec3(activeProgram->getParamLocation("lightPosition"), glm::vec3(finalMatrix[3]));
	// as for spot lights, with an arbitrary direction
	activeProgram->setVec3(activeProgram->getParamLocation("lightDirection"), glm::normalize(glm::mat3(finalMatrix) * glm::vec3(0.0f, -1.0f, 0.0f)));
	activeProgram->setVec3(uniforms["lightAmbient"], glm::vec3(getAmbient()));
	activeProgram->setVec3(uniforms["lightDiffuse"], glm::vec3(getDiffuse()));
	activeProgram->setVec3(uniforms["lightSpecular"], glm::vec3(getSpecular()));
	// as for spot lights, with 180° cut off like in OpenGL
	activeProgram->setFloat(activeProgram->getParamLocation("cutOff"), glm::radians(180.0f));
}

float LIB_API PointLight::getCutOff() {
	return cutOff;
}