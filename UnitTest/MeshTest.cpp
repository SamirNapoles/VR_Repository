#include "Test.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Material.h"
#include <GL/freeglut.h>

Material* emessiveWhite;

void meshMaterialTest() {

	GLfloat val[4];
	glGetMaterialfv(GL_FRONT, GL_EMISSION, val);
	assert(emessiveWhite->getEmission() == glm::make_vec4(val));

	glGetMaterialfv(GL_FRONT, GL_AMBIENT, val);
	assert(emessiveWhite->getAmbient() == glm::make_vec4(val));

	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, val);
	assert(emessiveWhite->getDiffuse() == glm::make_vec4(val));

	glGetMaterialfv(GL_FRONT, GL_SPECULAR, val);
	assert(emessiveWhite->getSpecular() == glm::make_vec4(val));

	GLfloat shin;
	glGetMaterialfv(GL_FRONT, GL_SHININESS, &shin);
	assert(emessiveWhite->getShininess() == shin);
}

void meshTest() {

	emessiveWhite = new Material(Object::getNextId(), std::string("emessiveWhite"),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
		glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
		glm::vec4(0.9f, 0.9f, 0.9f, 1.0f),
		1.0f
	);
	Mesh mesh = Mesh(Object::getNextId(), std::string("mesh"), std::shared_ptr<Material>{emessiveWhite});

	mesh.addVertex(new Vertex(
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	), 0);
	mesh.addVertex(new Vertex(
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	), 0);
	mesh.addVertex(new Vertex(
		glm::vec3(0.5f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	), 0);

	mesh.render(glm::mat4(1.0f));

	meshMaterialTest();
}