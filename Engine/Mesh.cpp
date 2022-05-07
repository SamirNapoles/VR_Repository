#include "Mesh.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Program.h"

LIB_API Mesh::Mesh(const int id, const std::string name, std::shared_ptr<Material> material) :
	Node{id, name}, material(material) {}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vertexVbo);
    glDeleteBuffers(1, &normalVbo);
    glDeleteBuffers(1, &textureVbo);
    glDeleteBuffers(1, &faceVbo);
    glDeleteVertexArrays(1, &vao);
}

bool Mesh::getCastShadow() const {
    return castShadow;
}

void Mesh::setCastShadow(bool castShadow) {
    this->castShadow = castShadow;
}

void Mesh::setVao(unsigned int vertexVbo, unsigned int normalVbo, unsigned int textureVbo, unsigned int faceVbo, unsigned int vao, unsigned int  faceNr) {
    this->vertexVbo = vertexVbo;
    this->normalVbo = normalVbo;
    this->textureVbo = textureVbo;
    this->faceVbo = faceVbo;
    this->vao = vao;
    this->faceNr = faceNr;
}

void LIB_API Mesh::render(glm::mat4 finalMatrix) {

    //Material
    Material* m = material.get();
    m->render(finalMatrix);

    // Set model matrix as current OpenGL matrix:
    Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], finalMatrix);
    Program::getActiveProgram()->setMatrix3(Program::getActiveProgram()->getUniforms()["modelviewInverseTranspose"], glm::inverseTranspose(glm::mat3(finalMatrix)));

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, faceNr * 3, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

float Mesh::getRadius() {
    return radius;
}

void Mesh::setRadius(float radius) {
    this->radius = radius;
}