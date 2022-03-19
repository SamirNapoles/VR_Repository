#include "Node.h"
#include <glm/glm.hpp>
#include <math.h>
#include <vector>

#ifndef ROBOT_ARM
#define ROBOT_ARM

class RobotArm {
public:
	RobotArm(std::vector<Node*> joints, std::vector<glm::vec3> jointsRotationLimit, std::vector<Node*> claws, std::vector<glm::vec3> clawsRotationLimit, Node* ball);
	~RobotArm();

public:
	void rotateJoint(glm::vec3 direction);
	void setRotationSpeed(float rotationSpeed);
	float getRotationSpeed();
	void setActiveJoint(int activeJoint);
	int getActiveJoint();
	void openClaws();
	void closeClaws();
	void update();

private:
	void rotateClaw(int clawIndex, glm::vec3 direction);
	void grab();

private:
	std::vector<Node*> joints;
	std::vector<glm::vec3> jointsRotationLimit;
	std::vector<glm::vec3> jointsRotation;
	std::vector<glm::mat4> jointsOriginalTransform;

	std::vector<Node*> claws;
	std::vector<glm::vec3> clawsRotationLimit;
	std::vector<glm::vec3> clawsRotation;
	std::vector<glm::mat4> clawsOriginalTransform;

	Node* originalBallParent;
	glm::mat4 originalBallTransform;
	Node* ball;
	bool release = true;
	bool hasBall = false;

	float clawDistance = 30.0f;

	float rotationSpeed = 1.0f;
	float fallSpeed = 3.0f;
	int activeJoint = 0;
};

#endif //ROBOT_ARM