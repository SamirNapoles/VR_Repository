#include "Test.h"
#include "Vertex.h"

void vertexTest() {

	Vertex v = Vertex(
		glm::vec3(10.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);

	assert(v.getPosition() == glm::vec3(10.0f, 0.0f, 0.0f));
	assert(v.getNormal() == glm::vec3(1.0f, 0.0f, 0.0f));

	v.setTextureCoordinates(glm::vec2(1.0f, 0.0f));
	assert(v.getTextureCoordinates() == glm::vec2(1.0f, 0.0f));
}