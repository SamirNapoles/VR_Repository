#include "Material.h"
#include <GL/freeglut.h>

#include "Program.h"

LIB_API Material::Material(int id, const std::string name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess) :
	Object{ id, name }, emission(emission), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), texture(nullptr) {}

LIB_API Material::Material(int id, const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess) :
	Material(id, name, glm::vec4(0.0f), ambient, diffuse, specular, shininess) {}

Material::~Material() {}

void LIB_API Material::render(glm::mat4 finalMatrix) {
	if (texture != NULL)
		texture->render(finalMatrix);

	Program* activeProgram = Program::getActiveProgram();
	std::map<std::string, int> uniforms = activeProgram->getUniforms();
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