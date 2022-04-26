#include "Mesh.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Program.h"

LIB_API Mesh::Mesh(const int id, const std::string name, std::shared_ptr<Material> material) :
	Node{id, name}, material(material) {}

Mesh::~Mesh() {
    //vertices.clear();
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

/*
void Mesh::addVertex(Vertex* v,int lod) {
    if (vertices.size() <= lod) {
        std::vector<Vertex*> tempVec;
        tempVec.push_back(v);
        vertices.push_back(tempVec);
    } else 
	    vertices.at(lod).push_back(v);
}

std::vector<Vertex*> Mesh::getVertices(int lod) {
    return vertices.at(lod);
}
*/

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
    if (m != nullptr)
        m->render(finalMatrix);

    // Set model matrix as current OpenGL matrix:
    Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], finalMatrix);
    Program::getActiveProgram()->setMatrix3(Program::getActiveProgram()->getUniforms()["modelviewInverseTranspose"], glm::inverseTranspose(glm::mat3(finalMatrix)));

    glBindVertexArray(vao);

    if (faceVbo != NULL)
        glDrawElements(GL_TRIANGLES, faceNr * 3, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, faceNr * 3);

    glBindVertexArray(0);
}