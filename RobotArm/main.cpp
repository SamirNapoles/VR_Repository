/*
* Forces windows to use dedicated instead of built-in GPU
* https://stackoverflow.com/questions/16823372/forcing-machine-to-use-dedicated-graphics-card
*/
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

/**
The cubemap image is the work of Emil Persson, aka Humus: http://www.humus.name
*/

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
bool stereoscopic = false;

glm::vec3 cameraPos = glm::vec3(0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);

float yaw = 90.0f;
float pitch = 0.0f;

float cameraSpeed = 0.3f;
float rotationSpeed = 5.0f;

float stationaryCameraX = -1.5f;
float stationaryCameraY = 3.0f;
float stationaryCameraZ = 2.5;
float stationaryRotationX = -25.0f;
float stationaryRotationY = -30.0f;
float stationaryRotationZ = -10.0f;

Node* root;
Hands* hands;
bool isActive = true;
RobotArm* ra;

Camera* freeCamera = nullptr;
Camera* stationaryCamera = nullptr;
Camera* activeCamera = nullptr;

std::vector<Node*> prevCollisions{};

void keyboardCallback(int key) {

	if (activeCamera == freeCamera && !stereoscopic) {
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
	else if (activeCamera == freeCamera) {
		//Free camera movement based on hmd
		switch (key) {
			case 'w':
				cameraPos += cameraSpeed * activeCamera->getFrontDirection();
				break;
			case 's':
				cameraPos -= cameraSpeed * activeCamera->getFrontDirection();
				break;
			case 'a':
				cameraPos -= cameraSpeed * activeCamera->getRightDirection();
				break;
			case 'd':
				cameraPos += cameraSpeed * activeCamera->getRightDirection();
				break;

			case 'e':
				cameraPos += cameraSpeed * cameraUp;
				break;
			case 'q':
				cameraPos -= cameraSpeed * cameraUp;
				break;
		}
	}

	switch (key) {

		//Camera management
		case 'c':
			//Valid only in normal rendering (non stereoscopic)
			if (!stereoscopic) {
				activeCamera = (activeCamera == freeCamera ? stationaryCamera : freeCamera);
				engine.setCamera(activeCamera);
			}
			break;

		//Application controls
		//Exit application
		case 27: // Esc
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
	if (activeCamera == freeCamera && !stereoscopic) {
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		cameraFront = glm::normalize(direction);
		cameraPos += activeCamera->getWorldPosition();

		activeCamera->setTransform(glm::inverse(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
	}
	else if (activeCamera == freeCamera)
		activeCamera->setWorldPosition(activeCamera->getWorldPosition() + cameraPos);

	ra->update();


	engine.getList()->addEntry(root, root->getTransform());
	//draw scene
	engine.getList()->render(activeCamera->getInverse());

	//Use engine's hands for controls
	std::vector<Node*> collisions = hands->getCollisions(root);
	std::vector<Node*>::iterator it;
	for (it = collisions.begin(); it != collisions.end(); it++) {
		
		std::string name = (*it)->getName();

		//Check if this collision was triggered last frame too
		bool prevColl = false;
		std::vector<Node*>::iterator pit;
		for (pit = prevCollisions.begin(); pit != prevCollisions.end(); pit++) {
			if ((*pit)->getName() == name) {
				prevColl = true;
				break;
			}
		}
		
		//Application controls
		if (name == "button_esc") //Exit application
			isActive = false;

		//Robot arm controls
		else if (name == "button_next_joint" && !prevColl)
			ra->setActiveJoint((ra->getActiveJoint() + 4 + 1) % 4);
		else if (name == "button_prev_joint" && !prevColl)
			ra->setActiveJoint((ra->getActiveJoint() + 4 - 1) % 4);
		else if (name == "up")
			ra->rotateJoint(glm::vec3(-1.0f, 0.0f, 0.0f));
		else if (name == "down")
			ra->rotateJoint(glm::vec3(1.0f, 0.0f, 0.0f));
		else if (name == "right")
			ra->rotateJoint(glm::vec3(0.0f, 1.0f, 0.0f));
		else if (name == "left")
			ra->rotateJoint(glm::vec3(0.0f, -1.0f, 0.0f));
		else if (name == "button_open_claw")
			ra->openClaws();
		else if (name == "button_close_claw")
			ra->closeClaws();
	}
	prevCollisions = collisions;

	//clear() would delete pointers which would probably erease scene graph; to consider use of shared pointers in ListNode
	engine.getList()->removeAllEntries();
}

/**
 * Application entry point.
 * @param argc number of command-line arguments passed
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char* argv[])
{
	engine = Engine(stereoscopic);
	engine.setTexturePath("./newScene/");
	engine.init("RobotArm", keyboardCallback, displayCallback, 0.75f);
	root = engine.loadScene("./newScene/scene.OVO");

	freeCamera = (Camera*)root->findByName("freeCamera");
	stationaryCamera = (Camera*)root->findByName("stationaryCamera");
	//activeCamera = stereoscopic ? freeCamera : stationaryCamera;
	activeCamera = freeCamera;
	engine.setCamera(activeCamera);

	hands = engine.getHands();

	if (!stereoscopic) {
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
		stationaryCamera->setTransform(translation_cam * rotationX_cam * rotationY_cam * rotationZ_cam);
		cameraPos.y = stationaryCameraY;
	}

	//Set where the fake shadows will be projected
	Mesh* floor = (Mesh*)root->findByName("Floor");
	Mesh* table = (Mesh*)root->findByName("Table");
	((FakeShadow*)root->findByName("Table_shadow"))			->setShadowParent(floor);
	((FakeShadow*)root->findByName("Teapot_shadow"))		->setShadowParent(table);
	((FakeShadow*)root->findByName("TV_shadow"))			->setShadowParent(floor);
	((FakeShadow*)root->findByName("arm1_shadow"))			->setShadowParent(floor);
	((FakeShadow*)root->findByName("arm2_shadow"))			->setShadowParent(floor);
	((FakeShadow*)root->findByName("arm3_shadow"))			->setShadowParent(floor);
	((FakeShadow*)root->findByName("clawSupport_shadow"))	->setShadowParent(floor);
	((FakeShadow*)root->findByName("clawL_shadow"))			->setShadowParent(floor);
	((FakeShadow*)root->findByName("clawR_shadow"))			->setShadowParent(floor);
	((FakeShadow*)root->findByName("Sphere_shadow"))		->setShadowParent(floor);
	((FakeShadow*)root->findByName("joystick_shadow"))		->setShadowParent(floor);

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
		glm::vec3(0.0f, 15.0f, 0.0f),
		glm::vec3(0.0f, 15.0f, 0.0f)
	};
	ra = new RobotArm(joints, jointsLimits, claws, clawsLimits, ball);

	while (isActive) {
		engine.begin();
		cameraPos = glm::vec3(0.0f);
	}

	engine.free();

	// Done:
	std::cout << "\n[application terminated]" << std::endl;
	return 0;
}
