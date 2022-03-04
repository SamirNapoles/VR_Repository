#include "Test.h"
#include "Object.h"

void idTest() {
	int id = Object::getNextId();
	//3 because there are other 2 empty constructor called before
	assert(id == 4);
}

void objectTest() {

	idTest();

	int id = Object::getNextId();
	Object o = Object(id, std::to_string(id));
	assert(o.getId() == 5);
	assert(o.getName() == "5");

	o.setName("abc");
	assert(o.getName() == "abc");
}