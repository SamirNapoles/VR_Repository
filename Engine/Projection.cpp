#include "Projection.h"
#include <GL/freeglut.h>

// #include "engine.h"
#include "Program.h"

Projection::Projection(int width, int heigth) :
	width(width), heigth(heigth) {

    setOpenGLProjection();
}
Projection::~Projection() {};


void LIB_API Projection::setOpenGLProjection() {
    // Set projection matrix:
    /*glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);*/

    /*
    Engine::getProgramOmni()->setMatrix(Engine::getProjectionMatrixOmni(), projection);
    Engine::getProgramDirectional()->setMatrix(Engine::getProjectionMatrixDirectional(), projection);
    Engine::getProgramSpot()->setMatrix(Engine::getProjectionMatrixSpot(), projection);
    */
    Program::getActiveProgram()->setMatrix(Program::getUniforms()["projection"], projection);
}

void Projection::setProjection(glm::mat4 projection) {
    this->projection = projection;
}
glm::mat4 LIB_API Projection::getProjection() {
    return projection;
}

int LIB_API Projection::getWidth() {
    return width;
}
int LIB_API Projection::getHeigth() {
    return heigth;
}

void Projection::setWidth(int width) {
    this->width = width;
}
void Projection::setHeigth(int heigth) {
    this->heigth = heigth;
}

void Projection::update() {}