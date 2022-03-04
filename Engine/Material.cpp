#include "Material.h"
#include <GL/freeglut.h>

LIB_API Material::Material(int id, const std::string name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess) :
	Object{ id, name }, emission(emission), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), texture(nullptr) {}

LIB_API Material::Material(int id, const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess) :
	Material(id, name, glm::vec4(0.0f), ambient, diffuse, specular, shininess) {}

Material::~Material() {}

void LIB_API Material::render(glm::mat4 cameraInv) {
	if(texture != NULL)
		texture->render(cameraInv);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(getEmission()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(getAmbient()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(getSpecular()));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, getShininess());
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