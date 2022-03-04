#include "UIProjection.h"
#include <GL/freeglut.h>

LIB_API UIProjection::UIProjection(int width, int heigth) :
	UIProjection(width, heigth, 0.0f, (float)width, (float)heigth, 0.0f, -1.0f, 1.0f) {};

LIB_API UIProjection::UIProjection(int width, int heigth, float left, float right, float bottom, float top, float zNear, float zFar) :
	OrthogonalProjection{ width, heigth, left, right, bottom, top, zNear, zFar } {

	update();
};
LIB_API UIProjection::~UIProjection() {}

void LIB_API UIProjection::setColor(glm::vec3 color) {
	this->color = color;
}

void LIB_API UIProjection::addLabel(std::string label) {
	labels.push_back(label);
}

void LIB_API UIProjection::editLabel(int index, std::string label) {
	labels.at(index) = label;
}

void LIB_API UIProjection::clear() {
	labels.clear();
}

void LIB_API UIProjection::print() {

	bool lighting = glIsEnabled(GL_LIGHTING);
	if (lighting) glDisable(GL_LIGHTING);

	setOpenGLProjection();
	glLoadIdentity();
	glColor3fv(glm::value_ptr(color));

	float labelHeigth = 25.0f;
	float startY = labelHeigth;
	for (int i = 0; i < labels.size(); i++) {
		float y = startY + labelHeigth * i;
		glRasterPos2f(0.0f, y);
		glutBitmapString(GLUT_BITMAP_8_BY_13, reinterpret_cast<const unsigned char*>(labels.at(i).c_str()));
	}

	if (lighting) glEnable(GL_LIGHTING);
}