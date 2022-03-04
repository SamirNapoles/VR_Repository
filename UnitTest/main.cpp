#include "Test.h"
#include <GL/freeglut.h>

int main() {

	//initialize a fake opengl context
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	int argc = 0;
	char** argv = nullptr;
	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutCreateWindow("");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	std::cout << "Unit Testing" << std::endl;
	std::cout << "------------" << std::endl;

	std::cout << std::endl;
	std::cout << "Engine" << std::endl;
	std::cout << "=======================================" << std::endl;

	//Object class testing
	objectTest();
	std::cout << "Object:\t\tOK" << std::endl;

	//Node class testing
	nodeTest();
	std::cout << "Node:\t\tOK" << std::endl;

	//Projection classes testing
	projectionTest();
	std::cout << "Projections:\tOK" << std::endl;

	//Camera class testing
	cameraTest();
	std::cout << "Camera:\t\tOK" << std::endl;

	//Light classes testing
	lightTest();
	std::cout << "Lights:\t\tOK" << std::endl;

	//Material class testing
	materialTest();
	std::cout << "Material:\tOK" << std::endl;

	//Vertex class testing
	vertexTest();
	std::cout << "Vertex:\t\tOK" << std::endl;

	//Mesh class testing
	meshTest();
	std::cout << "Mesh:\t\tOK" << std::endl;

	//FileReader class testing
	fileReaderTest();
	std::cout << "FileReader:\tOK" << std::endl;

	//List class testing
	listTest();
	std::cout << "List:\t\tOK" << std::endl;

	//Fake shadow class testing
	fakeShadowTest();
	std::cout << "FakeShadow:\tOK" << std::endl;

	std::cout << "RobotArm" << std::endl;
	std::cout << "=======================================" << std::endl;

	return 0;
}
