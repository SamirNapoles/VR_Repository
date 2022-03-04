#include "Vertex.h"

LIB_API Vertex::Vertex(glm::vec3 position, glm::vec3 normal) :
	position(position), normal(normal), textureCoordinates(glm::vec2(0.0f)) {}

Vertex::~Vertex() {}

glm::vec3 Vertex::getPosition() {
	return position;
}
glm::vec3 Vertex::getNormal() {
	return normal;
}
glm::vec2 Vertex::getTextureCoordinates() {
	return textureCoordinates;
}
void Vertex::setTextureCoordinates(glm::vec2 textCoord) {
	textureCoordinates = textCoord;
}