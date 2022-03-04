#include "Test.h"
#include "OrthogonalProjection.h"
#include "PerspectiveProjection.h"

void perspectiveProjectionTest() {

	Projection proj = PerspectiveProjection(1920, 1080);

	assert(proj.getWidth() == 1920);
	assert(proj.getHeigth() == 1080);

	glm::mat4 p = glm::perspective(
		glm::radians(45.0f),
		1920 / 1080.0f,
		1.0f,
		100.0f
	);
	assert(proj.getProjection() == p);
}

void orthogonalProjectionTest() {

	Projection proj = OrthogonalProjection(1920, 1080);

	assert(proj.getWidth() == 1920);
	assert(proj.getHeigth() == 1080);

	glm::mat4 p = glm::ortho(
		0.0f,
		1920.0f,
		1080.0f,
		0.0f,
		-1.0f,
		1.0f
	);
	assert(proj.getProjection() == p);
}

void projectionTest() {

	perspectiveProjectionTest();

	orthogonalProjectionTest();
}