#include "Object.h"

LIB_API Object::Object(int id, const std::string name) :
	id(id), name(name) {}

Object::~Object() {}

int Object::nextId = 1;
int LIB_API Object::getNextId() {
	return Object::nextId++;
}

int LIB_API Object::getId() const {
	return Object::id;
}

std::string LIB_API Object::getName() const {
	return Object::name;
}

void LIB_API Object::setName(std::string name) {
	Object::name = name;
}

void LIB_API Object::render(glm::mat4 cameraInv) {
	std::cout << "Object={"
		<< "id=" << Object::id << ", "
		<< "name=" << Object::name
		<< "}";
}