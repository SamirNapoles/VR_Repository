#include "Test.h"
#include "FileReader.h"
#include <GL/freeglut.h>
#include <List.h>


void fileReaderTest() {
	List list = List();
	FileReader filereader = FileReader();
	Node* root = filereader.readFile("..\\RobotArm\\examples\\simple3dScene.ovo");
	list.addEntry(root);

	assert(root->getName() == "[root]");
	assert(root->getNumberOfChildren() == 11);
	assert(root->getNthChild(0)->getName() == "Box001");

	list.clear();
}