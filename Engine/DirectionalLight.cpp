#include "DirectionalLight.h"

#include "Program.h"

LIB_API DirectionalLight::DirectionalLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular) :
	Light{ id, name, lightNumber, ambient, diffuse, specular } {}

void LIB_API DirectionalLight::render(glm::mat4 finalMatrix) {

	glm::vec4 pos = getPosition();
	pos[3] = 0.0f;
	setPosition(pos);

	Program* activeProgram = Program::getActiveProgram();
	std::map<std::string, int> uniforms = activeProgram->getUniforms();
	activeProgram->setVec3(activeProgram->getParamLocation("lightDirection"), glm::normalize(glm::mat3(finalMatrix) * glm::vec3(getPosition())));
	activeProgram->setVec3(uniforms["lightAmbient"], glm::vec3(getAmbient()));
	activeProgram->setVec3(uniforms["lightDiffuse"], glm::vec3(getDiffuse()));
	activeProgram->setVec3(uniforms["lightSpecular"], glm::vec3(getSpecular()));
}

void LIB_API DirectionalLight::setTransform(glm::mat4 transform) {
	glm::vec4 pos = transform[3];
	pos[3] = 0.0f;
	setPosition(pos);

	Node::setTransform(transform);
}