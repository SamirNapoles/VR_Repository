#include "Test.h"
#include "PerspectiveProjection.h"
#include "Camera.h"

Camera* c;
Projection* p;

void projectionGetterTest() {
	assert(c->getProjection() == p);
}

void inverseTest() {

	glm::mat4 translation = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);

	int id = Object::getNextId();
	Node* root = new Node(id, std::to_string(id));
	root->setTransform(translation);
	root->addChild(c);

	assert(c->getInverse() == glm::inverse(translation));
}

void cameraTest() {

	int id = Object::getNextId();
	p = new PerspectiveProjection(1920, 1080);
	c = new Camera(id, std::to_string(id), p);

	projectionGetterTest();

	inverseTest();
}