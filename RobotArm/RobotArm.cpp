#include "RobotArm.h"

RobotArm::RobotArm(std::vector<Node*> joints, std::vector<glm::vec3> jointsRotationLimit, std::vector<Node*> claws, std::vector<glm::vec3> clawsRotationLimit, Node* ball) :
	joints(joints), jointsRotationLimit(jointsRotationLimit), claws(claws), clawsRotationLimit(clawsRotationLimit), ball(ball) {

	//Initialize joints rotation
	jointsRotation = std::vector<glm::vec3>(jointsRotationLimit.size(), glm::vec3(0.0f));
	for (int i = 0; i < joints.size(); i++)
		jointsOriginalTransform.push_back(joints.at(i)->getTransform());

	//Initialize claws rotation
	clawsRotation = std::vector<glm::vec3>(clawsRotationLimit.size(), glm::vec3(0.0f));
	for (int i = 0; i < claws.size(); i++) {
		glm::mat4 original = claws.at(i)->getTransform();
		clawsOriginalTransform.push_back(original);

		//Sets by default claws max opening
		glm::vec3 newRotation = glm::vec3(clawsRotationLimit.at(i));
		newRotation.y *= (i % 2 == 0 ? 1.0f : -1.0f);
		glm::mat4 ry = glm::rotate(
			glm::mat4(1.0f),
			glm::radians(newRotation.y),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		claws.at(i)->setTransform(original * ry);
		clawsRotation.at(i) = newRotation;
	}

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

	glm::vec3 ballPosition = glm::vec3(ball->getWorldPosition());
	glm::vec3 clawPosition = glm::vec3(joints.at(3)->getWorldPosition());

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
		float ballRadius = ((Mesh*)ball)->getRadius() + 0.05f;
		joints.at(3)->addChild(ball);
		ball->setTransform(
			glm::scale(
				glm::mat4(1.0f),
				glm::vec3(map, 1.0f, 1.0f)
			) *
			glm::translate(
				glm::mat4(1.0f),
				glm::vec3(0.0f, ballRadius, ballRadius)
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

		if (ballPosition.y >= ((Mesh*)ball)->getRadius()) {
			ball->setTransform(
				ball->getTransform() *
				glm::translate(
					glm::mat4(1.0f),
					glm::vec3(0.0f, -0.03f, 0.0f) * fallSpeed
				)
			);
		}
	}
}

void RobotArm::rotateJoint(glm::vec3 direction) {

	glm::vec3* activeRotation = &jointsRotation.at(activeJoint);
	glm::vec3* activeRotationLimit = &jointsRotationLimit.at(activeJoint);

	glm::mat4 r = rotate(activeRotation, activeRotationLimit, direction);

	//Apply rotations
	joints.at(activeJoint)->setTransform(
		jointsOriginalTransform.at(activeJoint) * r
	);
}

void RobotArm::rotateClaw(int clawIndex, glm::vec3 direction) {

	glm::vec3* activeRotation = &clawsRotation.at(clawIndex);
	glm::vec3* activeRotationLimit = &clawsRotationLimit.at(clawIndex);

	glm::mat4 r = rotate(activeRotation, activeRotationLimit, direction);

	//Apply rotations
	claws.at(clawIndex)->setTransform(
		clawsOriginalTransform.at(clawIndex) * r
	);
}

glm::mat4 RobotArm::rotate(glm::vec3* rotation, glm::vec3* limit, glm::vec3 direction) {
	
	//Increase angle
	*rotation += direction * rotationSpeed;

	//Check for upper bound angle
	rotation->x = limit->x == 360.0f ? rotation->x : std::max(rotation->x, -(limit->x));
	rotation->y = limit->y == 360.0f ? rotation->y : std::max(rotation->y, -(limit->y));
	rotation->z = limit->z == 360.0f ? rotation->z : std::max(rotation->z, -(limit->z));

	//Check for lower bound angle
	rotation->x = limit->x == 360.0f ? rotation->x : std::min(rotation->x, limit->x);
	rotation->y = limit->y == 360.0f ? rotation->y : std::min(rotation->y, limit->y);
	rotation->z = limit->z == 360.0f ? rotation->z : std::min(rotation->z, limit->z);

	//Calculate rotations
	glm::mat4 rx = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(rotation->x),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	glm::mat4 ry = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(rotation->y),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	glm::mat4 rz = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(rotation->z),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	glm::mat4 r = rx * ry * rz;

	return r;
}