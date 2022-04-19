#include "SpotLight.h"
#include <GL/freeglut.h>

#include "Program.h"

LIB_API SpotLight::SpotLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, 
	const glm::vec4 diffuse, const glm::vec4 specular, const float cutOff, const glm::vec3 direction) :
	PointLight{ id, name, lightNumber, ambient, diffuse, specular, cutOff }, direction(direction) {};

void LIB_API SpotLight::render(glm::mat4 finalMatrix) {
	Program* activeProgram = Program::getActiveProgram();
	std::map<std::string, int> uniforms = activeProgram->getUniforms();
	
	activeProgram->setVec3(activeProgram->getParamLocation("lightPosition"), glm::vec3(finalMatrix[3]));
	activeProgram->setVec3(activeProgram->getParamLocation("lightDirection"), glm::normalize(glm::mat3(finalMatrix) * direction));
	activeProgram->setVec3(uniforms["lightAmbient"], glm::vec3(getAmbient()));
	activeProgram->setVec3(uniforms["lightDiffuse"], glm::vec3(getDiffuse()));
	activeProgram->setVec3(uniforms["lightSpecular"], glm::vec3(getSpecular()));
	activeProgram->setFloat(activeProgram->getParamLocation("cutOff"), glm::radians(getCutOff()));
}