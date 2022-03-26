#include "PointLight.h"
#include <GL/freeglut.h>

// #include "engine.h"
#include "Program.h"

LIB_API PointLight::PointLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, 
	const glm::vec4 diffuse, const glm::vec4 specular, const float cutOff) :
	Light{ id, name, lightNumber, ambient, diffuse, specular}, cutOff(cutOff) {};

void LIB_API PointLight::render(glm::mat4 finalMatrix) {
	glLightfv(getLightNumber(), GL_SPOT_CUTOFF, &cutOff);

	/*
	Engine::getProgramOmni()->setMatrix(Engine::getModelViewMatrixOmni(), finalMatrix);
	Engine::getProgramOmni()->setMatrix3(Engine::getInverseTransposeOmni(), glm::inverseTranspose(glm::mat3(finalMatrix)));
	Engine::getProgramOmni()->setVec3(Engine::getProgramOmni()->getParamLocation("lightPosition"), glm::vec3(getPosition()));
	Engine::getProgramOmni()->setVec3(Engine::getProgramOmni()->getParamLocation("lightAmbient"), glm::vec3(getAmbient()));
	Engine::getProgramOmni()->setVec3(Engine::getProgramOmni()->getParamLocation("lightDiffuse"), glm::vec3(getDiffuse()));
	Engine::getProgramOmni()->setVec3(Engine::getProgramOmni()->getParamLocation("lightSpecular"), glm::vec3(getSpecular()));
	*/

	Program* activeProgram = Program::getActiveProgram();
	std::map<std::string, int> uniforms = activeProgram->getUniforms();
	//activeProgram->setMatrix(uniforms["modelview"], finalMatrix);
	//activeProgram->setMatrix3(uniforms["modelviewInverseTranspose"], glm::inverseTranspose(glm::mat3(finalMatrix)));
	activeProgram->setVec3(activeProgram->getParamLocation("lightPosition"), glm::vec3(finalMatrix[3]));
	activeProgram->setVec3(uniforms["lightAmbient"], glm::vec3(getAmbient()));
	activeProgram->setVec3(uniforms["lightDiffuse"], glm::vec3(getDiffuse()));
	activeProgram->setVec3(uniforms["lightSpecular"], glm::vec3(getSpecular()));

	Light::render(finalMatrix);
}

float LIB_API PointLight::getCutOff() {
	return cutOff;
}