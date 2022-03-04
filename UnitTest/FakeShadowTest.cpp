#include "Test.h"
#include "FakeShadow.h"
#include "Vertex.h"
#include <GL/freeglut.h>


void fakeShadowTest() {
	Material* emessiveWhite = new Material(Object::getNextId(), std::string("emessiveWhite"),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
		glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
		glm::vec4(0.9f, 0.9f, 0.9f, 1.0f),
		1.0f
	);
	Mesh mesh = Mesh(Object::getNextId(), std::string("mesh"), std::shared_ptr<Material>{emessiveWhite});
	FakeShadow shadow = FakeShadow(Object::getNextId(), std::string("shadow"), nullptr, &mesh);
	shadow.setOffset(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	shadow.addVertex(new Vertex(
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	), 0);
	shadow.addVertex(new Vertex(
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	), 0);
	shadow.addVertex(new Vertex(
		glm::vec3(0.5f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	), 0);

	shadow.render(glm::mat4(1.0f));

	assert(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f)) == shadow.getScaling());
}