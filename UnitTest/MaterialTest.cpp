#include "Test.h"
#include "Material.h"

void materialTest() {
	
	Material emessiveWhite = Material(Object::getNextId(), std::string("emessiveWhite"),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
		glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
		glm::vec4(0.9f, 0.9f, 0.9f, 1.0f),
		1
	);
	assert(emessiveWhite.getEmission() == glm::vec4(1.0f));
	assert(emessiveWhite.getAmbient() == glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	assert(emessiveWhite.getDiffuse() == glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
	assert(emessiveWhite.getSpecular() == glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
	assert(emessiveWhite.getShininess() == 1);

	Material normalWhite = Material(Object::getNextId(), std::string("normalWhite"),
		glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
		glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
		glm::vec4(0.9f, 0.9f, 0.9f, 1.0f),
		1
	);
	assert(normalWhite.getEmission() == glm::vec4(0.0f));
	assert(normalWhite.getAmbient() == glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	assert(normalWhite.getDiffuse() == glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
	assert(normalWhite.getSpecular() == glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
	assert(normalWhite.getShininess() == 1);
}