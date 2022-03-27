#include "Material.h"
#include <GL/freeglut.h>

// #include "engine.h"
#include "Program.h"

LIB_API Material::Material(int id, const std::string name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess) :
	Object{ id, name }, emission(emission), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), texture(nullptr) {}

LIB_API Material::Material(int id, const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess) :
	Material(id, name, glm::vec4(0.0f), ambient, diffuse, specular, shininess) {}

Material::~Material() {}

void LIB_API Material::render(glm::mat4 finalMatrix) {
	if (texture != NULL)
		texture->render(finalMatrix);
	/*
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(getEmission()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(getAmbient()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(getSpecular()));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, getShininess());
	*/

	/*
	Engine::getProgramOmni()->setVec3(Engine::getProgramOmni()->getParamLocation("materialEmission"), glm::vec3(getEmission()));
	Engine::getProgramOmni()->setVec3(Engine::getProgramOmni()->getParamLocation("materialAmbient"), glm::vec3(getAmbient()));
	Engine::getProgramOmni()->setVec3(Engine::getProgramOmni()->getParamLocation("materialDiffuse"), glm::vec3(getDiffuse()));
	Engine::getProgramOmni()->setVec3(Engine::getProgramOmni()->getParamLocation("materialSpecular"), glm::vec3(getSpecular()));
	Engine::getProgramOmni()->setFloat(Engine::getProgramOmni()->getParamLocation("materialShiniess"), shininess);

	Engine::getProgramDirectional()->setVec3(Engine::getProgramDirectional()->getParamLocation("materialEmission"), glm::vec3(getEmission()));
	Engine::getProgramDirectional()->setVec3(Engine::getProgramDirectional()->getParamLocation("materialAmbient"), glm::vec3(getAmbient()));
	Engine::getProgramDirectional()->setVec3(Engine::getProgramDirectional()->getParamLocation("materialDiffuse"), glm::vec3(getDiffuse()));
	Engine::getProgramDirectional()->setVec3(Engine::getProgramDirectional()->getParamLocation("materialSpecular"), glm::vec3(getSpecular()));
	Engine::getProgramDirectional()->setFloat(Engine::getProgramDirectional()->getParamLocation("materialShiniess"), shininess);

	Engine::getProgramSpot()->setVec3(Engine::getProgramSpot()->getParamLocation("materialEmission"), glm::vec3(getEmission()));
	Engine::getProgramSpot()->setVec3(Engine::getProgramSpot()->getParamLocation("materialAmbient"), glm::vec3(getAmbient()));
	Engine::getProgramSpot()->setVec3(Engine::getProgramSpot()->getParamLocation("materialDiffuse"), glm::vec3(getDiffuse()));
	Engine::getProgramSpot()->setVec3(Engine::getProgramSpot()->getParamLocation("materialSpecular"), glm::vec3(getSpecular()));
	Engine::getProgramSpot()->setFloat(Engine::getProgramSpot()->getParamLocation("materialShiniess"), shininess);
	*/

	Program* activeProgram = Program::getActiveProgram();
	std::map<std::string, int> uniforms = Program::getUniforms();
	activeProgram->setVec3(uniforms["materialEmission"], glm::vec3(getEmission()));
	activeProgram->setVec3(uniforms["materialAmbient"], glm::vec3(getAmbient()));
	activeProgram->setVec3(uniforms["materialDiffuse"], glm::vec3(getDiffuse()));
	activeProgram->setVec3(uniforms["materialSpecular"], glm::vec3(getSpecular()));
	activeProgram->setFloat(uniforms["materialShiniess"], shininess);
}

glm::vec4 LIB_API Material::getEmission() {
	return emission;
}
glm::vec4 LIB_API Material::getAmbient() {
	return ambient;
}
glm::vec4 LIB_API Material::getDiffuse() {
	return diffuse;
}
glm::vec4 LIB_API Material::getSpecular() {
	return specular;
}
float LIB_API Material::getShininess() {
	return shininess;
}

void LIB_API Material::setTexture(Texture* t) {
	texture = t;
}
Texture LIB_API* Material::getTexture() {
	return texture;
}