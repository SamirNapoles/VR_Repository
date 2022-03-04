#include "Test.h"
#include "List.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <GL/freeglut.h>

//List used for render
List lightList = List();
void pointLightTest() {

	int lightNumber = Light::getNextLightNumber();
	assert(lightNumber == GL_LIGHT0);

	PointLight pl = PointLight(Object::getNextId(), std::string("pl"), lightNumber,
		glm::vec4(1.0f),
		glm::vec4(1.0f),
		glm::vec4(1.0f)
	);

	glm::vec3 pos = glm::vec3(10.0f, 0.0f, 1.0f);
	pl.setTransform(glm::translate(
		glm::mat4(1.0f),
		pos
	));
	glm::vec4 pos4 = glm::vec4(pos.x, pos.y, pos.z, 1.0f);
	assert(pl.getPosition() == pos4);

	pl.setConstantAttenuation(0.0f);
	pl.setLinearAttenuation(1.0f);
	pl.setQuadraticAttenuation(2.0f);
	assert(pl.getConstantAttenuation() == 0.0f);
	assert(pl.getLinearAttenuation() == 1.0f);
	assert(pl.getQuadraticAttenuation() == 2.0f);

	pl.render(glm::mat4(1.0f));

	GLfloat attenuation;
	glGetLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, &attenuation);
	assert(attenuation == 0.0f);
	glGetLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, &attenuation);
	assert(attenuation == 1.0f);
	glGetLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, &attenuation);
	assert(attenuation == 2.0f);

	GLfloat val[4];
	glGetLightfv(GL_LIGHT0, GL_POSITION, val);
	assert(glm::make_vec4(val) == glm::vec4(10.0f, 0.0f, 1.0f, 1.0f));

	assert(glIsEnabled(GL_LIGHTING));
	assert(glIsEnabled(GL_LIGHT0));
}

void spotLightTest() {

	Node* root = new Node(Object::getNextId(), "root");
	glm::vec3 rootPos = glm::vec3(1.0f);
	glm::vec4 rootPos4 = glm::vec4(1.0f);
	root->setTransform(glm::translate(
		glm::mat4(1.0f),
		rootPos
	));

	int lightNumber = Light::getNextLightNumber();
	assert(lightNumber == GL_LIGHT0);

	SpotLight* sl = new SpotLight(Object::getNextId(), std::string("sl"), lightNumber,
		glm::vec4(1.0f),
		glm::vec4(1.0f),
		glm::vec4(1.0f),
		1.0f,
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	root->addChild(sl);

	glm::vec3 pos = glm::vec3(10.0f, 0.0f, 1.0f);
	sl->setTransform(glm::translate(
		glm::mat4(1.0f),
		pos
	));
	glm::vec4 pos4 = glm::vec4(pos.x, pos.y, pos.z, 0.0f);
	assert(sl->getPosition() == rootPos4 + pos4);

	sl->setConstantAttenuation(0.0f);
	sl->setLinearAttenuation(1.0f);
	sl->setQuadraticAttenuation(2.0f);
	assert(sl->getConstantAttenuation() == 0.0f);
	assert(sl->getLinearAttenuation() == 1.0f);
	assert(sl->getQuadraticAttenuation() == 2.0f);

	lightList.addEntry(root);
	lightList.render(glm::mat4(1.0f));

	GLfloat attenuation;
	glGetLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, &attenuation);
	assert(attenuation == 0.0f);
	glGetLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, &attenuation);
	assert(attenuation == 1.0f);
	glGetLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, &attenuation);
	assert(attenuation == 2.0f);

	GLfloat val[4];
	glGetLightfv(GL_LIGHT0, GL_POSITION, val);
	assert(glm::make_vec4(val) == glm::vec4(11.0f, 1.0f, 2.0f, 1.0f));

	GLfloat direction[3];
	glGetLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	assert(glm::make_vec3(direction) == glm::vec3(0.0f, 0.0f, 1.0f));

	GLfloat cutOff;
	glGetLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &cutOff);
	assert(cutOff == 1.0f);

	assert(glIsEnabled(GL_LIGHTING));
	assert(glIsEnabled(GL_LIGHT0));
}

void directionalLightTest() {

	Node* node = new Node(Object::getNextId(), "node");
	glm::vec3 nodePos = glm::vec3(1.0f);
	node->setTransform(glm::translate(
		glm::mat4(1.0f),
		nodePos
	));

	int lightNumber = Light::getNextLightNumber();
	assert(lightNumber == GL_LIGHT1);

	DirectionalLight* dl = new DirectionalLight(Object::getNextId(), std::string("dl"), lightNumber,
		glm::vec4(1.0f),
		glm::vec4(1.0f),
		glm::vec4(1.0f)
	);
	dl->addChild(node);

	glm::vec3 pos = glm::vec3(10.0f, 0.0f, 1.0f);
	glm::vec4 pos4 = glm::vec4(10.0f, 0.0f, 1.0f, 0.0f);
	dl->setTransform(glm::translate(
		glm::mat4(1.0f),
		pos
	));
	assert(dl->getPosition() == pos4);

	dl->setConstantAttenuation(0.0f);
	dl->setLinearAttenuation(1.0f);
	dl->setQuadraticAttenuation(2.0f);
	assert(dl->getConstantAttenuation() == 0.0f);
	assert(dl->getLinearAttenuation() == 1.0f);
	assert(dl->getQuadraticAttenuation() == 2.0f);

	dl->render(glm::mat4(1.0f));

	GLfloat attenuation;
	glGetLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, &attenuation);
	assert(attenuation == 0.0f);
	glGetLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, &attenuation);
	assert(attenuation == 1.0f);
	glGetLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, &attenuation);
	assert(attenuation == 2.0f);

	GLfloat val[4];
	glGetLightfv(GL_LIGHT1, GL_POSITION, val);
	assert(glm::make_vec4(val) == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	assert(glIsEnabled(GL_LIGHTING));
	assert(glIsEnabled(GL_LIGHT1));
	delete (Light *)(dl);
}

void lightTest() {

	pointLightTest();

	spotLightTest();

	directionalLightTest();
	lightList.clear();
}
