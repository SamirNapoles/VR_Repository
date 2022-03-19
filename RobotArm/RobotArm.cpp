#include "RobotArm.h"

RobotArm::RobotArm(std::vector<Node*> joints, std::vector<glm::vec3> jointsRotationLimit, std::vector<Node*> claws, std::vector<glm::vec3> clawsRotationLimit, Node* ball) :
	joints(joints), jointsRotationLimit(jointsRotationLimit), claws(claws), clawsRotationLimit(clawsRotationLimit), ball(ball) {

	jointsRotation = std::vector<glm::vec3>(jointsRotationLimit.size(), glm::vec3(0.0f));
	for (int i = 0; i < joints.size(); i++)
		jointsOriginalTransform.push_back(joints.at(i)->getTransform());

	clawsRotation = std::vector<glm::vec3>(clawsRotationLimit.size(), glm::vec3(0.0f));
	for (int i = 0; i < claws.size(); i++)
		clawsOriginalTransform.push_back(claws.at(i)->getTransform());

	originalBallParent = ball->getParent();
	originalBallTransform = ball->getTransform();
};

RobotArm::~RobotArm() {};

void RobotArm::setRotationSpeed(float rotationSpeed) {
	this->rotationSpeed = rotationSpeed;
}

float RobotArm::getRotationSpeed() {
	return rotationSpeed;
}

void RobotArm::setActiveJoint(int activeJoint) {
	if (activeJoint >= joints.size() || activeJoint < 0)
		return;

	this->activeJoint = activeJoint;
}

int RobotArm::getActiveJoint() {
	return activeJoint;
}

void RobotArm::openClaws() {
	glm::vec3 dir(0.0f, 1.0f, 0.0f);
	rotateClaw(0, dir);
	rotateClaw(1, dir * -1.0f);
	grab();
}

void RobotArm::closeClaws() {
	glm::vec3 dir(0.0f, 1.0f, 0.0f);
	rotateClaw(0, dir * -1.0f);
	rotateClaw(1, dir);
	grab();
}

void RobotArm::grab() {

	glm::vec3 ballPosition = glm::vec3(ball->getFinalMatrix()[3]);
	glm::vec3 clawPosition = glm::vec3(joints.at(3)->getFinalMatrix()[3]);

	//Remap claws rotation to evaluate how much they are open
	float start1 = clawsRotationLimit.at(0).y;
	float stop1 = start1 * -1.0f;
	float start2 = 1.0f;
	float stop2 = 0.0f;
	float value = clawsRotation.at(0).y;
	float map = start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));

	if (hasBall && map == 1.0f) {
		release = true;
	}
	else if (glm::distance(ballPosition, clawPosition) <= clawDistance && map < 1.0f) {

		joints.at(3)->addChild(ball);
		ball->setTransform(
			glm::scale(
				glm::mat4(1.0f),
				glm::vec3(map, 1.0f, 1.0f)
			) *
			glm::translate(
				glm::mat4(1.0f),
				glm::vec3(0.0f, 10.0f, 10.0f)
			)
		);

		release = false;
		hasBall = true;
	}
}

void RobotArm::update() {
	if (release) {

		glm::vec3 ballPosition = ball->getWorldPosition();
		if (hasBall) {
			ball->setTransform(glm::translate(
				glm::mat4(1.0f),
				ballPosition
			));
			originalBallParent->addChild(ball);
			hasBall = false;
		}

		if (ballPosition.y >= 8.5) {
			ball->setTransform(
				ball->getTransform() *
				glm::translate(
					glm::mat4(1.0f),
					glm::vec3(0.0f, -1.0f, 0.0f) * fallSpeed
				)
			);
		}
	}
}

void RobotArm::rotateJoint(glm::vec3 direction) {

	glm::vec3* activeRotation = &jointsRotation.at(activeJoint);
	glm::vec3* activeRotationLimit = &jointsRotationLimit.at(activeJoint);

	//Increase angle
	*activeRotation += direction * rotationSpeed;

	//Check for upper bound angle
	activeRotation->x = activeRotationLimit->x == 360.0f ? activeRotation->x : std::max(activeRotation->x, -(activeRotationLimit->x));
	activeRotation->y = activeRotationLimit->y == 360.0f ? activeRotation->y : std::max(activeRotation->y, -(activeRotationLimit->y));
	activeRotation->z = activeRotationLimit->z == 360.0f ? activeRotation->z : std::max(activeRotation->z, -(activeRotationLimit->z));

	//Check for lower bound angle
	activeRotation->x = activeRotationLimit->x == 360.0f ? activeRotation->x : std::min(activeRotation->x, activeRotationLimit->x);
	activeRotation->y = activeRotationLimit->y == 360.0f ? activeRotation->y : std::min(activeRotation->y, activeRotationLimit->y);
	activeRotation->z = activeRotationLimit->z == 360.0f ? activeRotation->z : std::min(activeRotation->z, activeRotationLimit->z);

	//Calculate rotations
	glm::mat4 rx = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(activeRotation->x),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	glm::mat4 ry = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(activeRotation->y),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	glm::mat4 rz = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(activeRotation->z),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	glm::mat4 r = rx * ry * rz;

	//Apply rotations
	joints.at(activeJoint)->setTransform(
		jointsOriginalTransform.at(activeJoint) * r
	);
}

void RobotArm::rotateClaw(int clawIndex, glm::vec3 direction) {
	glm::vec3* activeRotation = &clawsRotation.at(clawIndex);
	glm::vec3* activeRotationLimit = &clawsRotationLimit.at(clawIndex);

	//Increase angle
	*activeRotation += direction * rotationSpeed;

	//Check for upper bound angle
	activeRotation->x = activeRotationLimit->x == 360.0f ? activeRotation->x : std::max(activeRotation->x, -(activeRotationLimit->x));
	activeRotation->y = activeRotationLimit->y == 360.0f ? activeRotation->y : std::max(activeRotation->y, -(activeRotationLimit->y));
	activeRotation->z = activeRotationLimit->z == 360.0f ? activeRotation->z : std::max(activeRotation->z, -(activeRotationLimit->z));

	//Check for lower bound angle
	activeRotation->x = activeRotationLimit->x == 360.0f ? activeRotation->x : std::min(activeRotation->x, activeRotationLimit->x);
	activeRotation->y = activeRotationLimit->y == 360.0f ? activeRotation->y : std::min(activeRotation->y, activeRotationLimit->y);
	activeRotation->z = activeRotationLimit->z == 360.0f ? activeRotation->z : std::min(activeRotation->z, activeRotationLimit->z);

	//Calculate rotations
	glm::mat4 rx = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(activeRotation->x),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	glm::mat4 ry = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(activeRotation->y),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	glm::mat4 rz = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(activeRotation->z),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	glm::mat4 r = rx * ry * rz;

	//Apply rotations
	claws.at(clawIndex)->setTransform(
		clawsOriginalTransform.at(clawIndex) * r
	);
}