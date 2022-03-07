#include "Mesh.h"
#include <GL/freeglut.h>

LIB_API Mesh::Mesh(const int id, const std::string name, std::shared_ptr<Material> material) :
	Node{id, name}, material(material) {}

Mesh::~Mesh() {
    vertices.clear();
}

bool Mesh::getCastShadow() const {
    return castShadow;
}

void Mesh::setCastShadow(bool castShadow) {
    this->castShadow = castShadow;
}

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

void LIB_API Mesh::render(glm::mat4 finalMatrix) {

    //Material
    Material* m = material.get();
    m->render(finalMatrix);

    // Set model matrix as current OpenGL matrix:
    glLoadMatrixf(glm::value_ptr(finalMatrix));

    //Vertex rendering Counter Clock-Wise
    glFrontFace(GL_CCW);

    // Triangles rendering
    glBegin(GL_TRIANGLES);
    for (Vertex* v : vertices.at(lod)) {
        glNormal3fv(glm::value_ptr(v->getNormal()));
        glTexCoord2fv(glm::value_ptr(v->getTextureCoordinates()));
        glVertex3fv(glm::value_ptr(v->getPosition()));     
    }

    glEnd();

    glDisable(GL_TEXTURE_2D);
}