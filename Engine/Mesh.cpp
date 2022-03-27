#include "Mesh.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

// #include "engine.h"
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
    m->render(finalMatrix);

    // Set model matrix as current OpenGL matrix:
    // glLoadMatrixf(glm::value_ptr(finalMatrix));
    /*
    Engine::getProgramOmni()->setMatrix(Engine::getModelViewMatrixOmni(), finalMatrix);
    Engine::getProgramOmni()->setMatrix3(Engine::getInverseTransposeOmni(), glm::inverseTranspose(glm::mat3(finalMatrix)));
    Engine::getProgramDirectional()->setMatrix(Engine::getModelViewMatrixDirectional(), finalMatrix);
    Engine::getProgramDirectional()->setMatrix3(Engine::getInverseTransposeDirectional(), glm::inverseTranspose(glm::mat3(finalMatrix)));
    Engine::getProgramSpot()->setMatrix(Engine::getModelViewMatrixSpot(), finalMatrix);
    Engine::getProgramSpot()->setMatrix3(Engine::getInverseTransposeSpot(), glm::inverseTranspose(glm::mat3(finalMatrix)));
    */
    Program::getActiveProgram()->setMatrix(Program::getUniforms()["modelview"], finalMatrix);
    Program::getActiveProgram()->setMatrix3(Program::getUniforms()["modelviewInverseTranspose"], glm::inverseTranspose(glm::mat3(finalMatrix)));

    //Vertex rendering Counter Clock-Wise
    glFrontFace(GL_CCW);

    // Triangles rendering
    /*
    glBegin(GL_TRIANGLES);
    for (Vertex* v : vertices.at(lod)) {
        //glNormal3fv(glm::value_ptr(v->getNormal()));
        //glTexCoord2fv(glm::value_ptr(v->getTextureCoordinates()));
        //glVertex3fv(glm::value_ptr(v->getPosition()));     
    }
    glEnd();
    */

    glBindVertexArray(vao);

    //glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glVertexAttribPointer((GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
    glVertexAttribPointer((GLuint) 2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

   // glDrawArrays(GL_TRIANGLES, 0, faceNr);
    glDrawElements(GL_TRIANGLES, faceNr * 3, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    // future element rendering through its faces
    // glBindVertexArray(vao);
    // glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, nullptr);

    //glDisable(GL_TEXTURE_2D);
    //glEnd();

    //glDisable(GL_TEXTURE_2D);
}