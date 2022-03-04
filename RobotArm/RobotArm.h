#include "Node.h"
#include <glm/glm.hpp>
#include <math.h>
#include <vector>

#ifndef ROBOT_ARM
#define ROBOT_ARM

class RobotArm {
public:
	RobotArm(std::vector<Node*> joints, std::vector<glm::vec3> jointsRotationLimit, Node* ball);
	~RobotArm();

public:
	void rotateJoint(glm::vec3 direction);
	void setRotationSpeed(float rotationSpeed);
	float getRotationSpeed();
	void setActiveJoint(int activeJoint);
	int getActiveJoint();
	void claw();
	void update();

private:
	std::vector<Node*> joints;
	std::vector<glm::vec3> jointsRotationLimit;
	std::vector<glm::vec3> jointsRotation;
	std::vector<glm::mat4> jointsOriginalTransform;

	Node* originalBallParent;
	glm::mat4 originalBallTransform;
	Node* ball;
	bool release = true;
	bool hasBall = false;

	float clawDistance = 15.0f;

	float rotationSpeed = 1.0f;
	float fallSpeed = 1.0f;
	int activeJoint = 0;
};

#endif //ROBOT_ARM