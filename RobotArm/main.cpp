//////////////
// #INCLUDE //
//////////////

	// Library header:
#include "Engine.h"
#include "RobotArm.h"

// C/C++:
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <iterator>
#include <vector>



//////////
// MAIN //
//////////

Engine engine;

glm::vec3 cameraPos = glm::vec3(0.0f, 100.0f, -40.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);

float yaw = 90.0f;
float pitch = 0.0f;

float cameraSpeed = 2.0f;
float rotationSpeed = 2.0f;

float stationaryCameraX = -102.0f;
float stationaryCameraY = 91.0f;
float stationaryCameraZ = 136;
float stationaryRotationX = -22.0f;
float stationaryRotationY = -26.0f;
float stationaryRotationZ = -8.0f;

Node* root;
bool isActive = true;
RobotArm* ra;

Camera* freeCamera = nullptr;
Camera* stationaryCamera = nullptr;
Camera* activeCamera = nullptr;



void keyboardCallback(int key) {

	if (activeCamera == freeCamera) {
		switch (key) {
			//Free camera movement
			case 'w':
				cameraPos += cameraSpeed * cameraFront;
				break;
			case 's':
				cameraPos -= cameraSpeed * cameraFront;
				break;
			case 'a':
				cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
				break;
			case 'd':
				cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
				break;
			case 'e':
				cameraPos += cameraSpeed * cameraUp;
				break;
			case 'q':
				cameraPos -= cameraSpeed * cameraUp;
				break;

			//Free camera rotation
			case '8':
				pitch++;
				break;
			case '2':
				pitch--;
				break;
			case '6':
				yaw++;
				break;
			case '4':
				yaw--;
				break;
		}

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	switch (key) {

		//Camera management
		case 'c':
			activeCamera = (activeCamera == freeCamera ? stationaryCamera : freeCamera);
			engine.setCamera(activeCamera);
			break;

		//Application controls
		case 27:
			isActive = false;
			break;

		//Robot arm controls
		case '+':
			ra->setActiveJoint((ra->getActiveJoint() + 4 + 1) % 4);
			break;
		case '-':
			ra->setActiveJoint((ra->getActiveJoint() + 4 - 1) % 4);
			break;
		case KEY_UP:
			ra->rotateJoint(glm::vec3(-1.0f, 0.0f, 0.0f));
			break;
		case KEY_DOWN:
			ra->rotateJoint(glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		case KEY_RIGHT:
			ra->rotateJoint(glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case KEY_LEFT:
			ra->rotateJoint(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case '.':
			ra->openClaws();
			break;
		case ',':
			ra->closeClaws();
			break;
	}
}

void displayCallback() {
	// Clear the screen:
	glm::vec3 color(37.0f, 53.0f, 87.0f);
	engine.clean(glm::vec4(color.r / 255, color.g / 255, color.b / 255, 1.0f));

	//Set projection
	activeCamera->getProjection()->setOpenGLProjection();
	if (activeCamera == freeCamera) {	
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		cameraFront = glm::normalize(direction);

		activeCamera->setTransform(glm::inverse(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
	}

	ra->update();


	engine.getList()->addEntry(root, root->getTransform());
	//draw scene
	engine.getList()->render(activeCamera->getInverse());
	//clear() would delete pointers which would probably erease scene graph; to consider use of shared pointers in ListNode
	engine.getList()->removeAllEntries();

	// Swap this context's buffer:
	engine.swap();

	// Force rendering refresh:
	engine.forceRefresh();
}

/**
 * Application entry point.
 * @param argc number of command-line arguments passed
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char* argv[])
{
	engine = Engine();
	engine.setTexturePath("./scene/");
	engine.init("RobotArm", keyboardCallback, displayCallback);
	root = engine.loadScene("./scene/ProjectScene.OVO");

	freeCamera = (Camera*)root->findByName("freeCamera");
	stationaryCamera = (Camera*)root->findByName("stationaryCamera");
	activeCamera = stationaryCamera;
	glm::mat4 translation_cam = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(0.0f + stationaryCameraX, 0.0f + stationaryCameraY, 0.0f + stationaryCameraZ)
	);
	glm::mat4 rotationX_cam = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(stationaryRotationX),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	glm::mat4 rotationY_cam = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(stationaryRotationY),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	glm::mat4 rotationZ_cam = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(stationaryRotationZ),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	activeCamera->setTransform(translation_cam * rotationX_cam * rotationY_cam * rotationZ_cam);

	//Set where the fake shadows will be projected
	Mesh* floor = (Mesh*)root->findByName("Floor");
	Mesh* table = (Mesh*)root->findByName("Table");
	((FakeShadow*)root->findByName("Table_shadow"))->setShadowParent(floor);
	((FakeShadow*)root->findByName("Teapot_shadow"))->setShadowParent(table);
	((FakeShadow*)root->findByName("TV_shadow"))->setShadowParent(floor);
	((FakeShadow*)root->findByName("arm1_shadow"))->setShadowParent(floor);
	((FakeShadow*)root->findByName("arm2_shadow"))->setShadowParent(floor);
	((FakeShadow*)root->findByName("arm3_shadow"))->setShadowParent(floor);
	((FakeShadow*)root->findByName("clawSupport_shadow"))->setShadowParent(floor);
	((FakeShadow*)root->findByName("clawL_shadow"))->setShadowParent(floor);
	((FakeShadow*)root->findByName("clawR_shadow"))->setShadowParent(floor);
	((FakeShadow*)root->findByName("Sphere_shadow"))->setShadowParent(floor);

	//Prepare robotarm
	Node* ball = root->findByName("Sphere");
	Node* plane = floor->findByName("base");
	Node* joint0 = plane->findByName("arm1");
	Node* joint1 = joint0->findByName("arm2");
	Node* joint2 = joint1->findByName("arm3");
	Node* joint3 = joint2->findByName("clawSupport");
	std::vector<Node*> joints{ joint0, joint1, joint2, joint3 };
	std::vector<glm::vec3> jointsLimits{
		glm::vec3(0.0f, 360.0f, 0.0f),
		glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(90.0f, 0.0f, 0.0f),
		glm::vec3(90.0f, 0.0f, 0.0f)
	};
	Node* claw0 = joint3->findByName("clawL");
	Node* claw1 = joint3->findByName("clawR");
	std::vector<Node*> claws{ claw0, claw1 };
	std::vector<glm::vec3> clawsLimits{
		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.0f, 10.0f, 0.0f)
	};
	ra = new RobotArm(joints, jointsLimits, claws, clawsLimits, ball);

	while (isActive) {
		engine.begin();
	}

	engine.free();

	// Done:
	std::cout << "\n[application terminated]" << std::endl;
	return 0;
}
