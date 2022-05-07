#include "FakeShadow.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Program.h"


LIB_API FakeShadow::FakeShadow(const int id, const std::string name, std::shared_ptr<Material> material, Mesh* model) :
    Mesh{ id, name, material }, model(model)
{
    scaling = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f));
    this->radius = model->getRadius();
};

FakeShadow::~FakeShadow() {
    //vertices.clear();
    glDeleteBuffers(1, &vertexVbo);
    glDeleteBuffers(1, &normalVbo);
    glDeleteBuffers(1, &textureVbo);
    glDeleteBuffers(1, &faceVbo);
    glDeleteVertexArrays(1, &vao);
}

void FakeShadow::setOffset(glm::mat4 offset) {
    this->offset = offset;
}

glm::mat4 FakeShadow::getOffset() const {
    return offset;
}

glm::mat4 FakeShadow::getScaling() const {
    return scaling;
}

void LIB_API FakeShadow::setShadowParent(Mesh* node) {
    node->addChild(this);
    setCastShadow(true);
}

glm::mat4 FakeShadow::getRotationMatrix() {

    glm::mat4 transf = model->getFinalMatrix();
    float xScale = glm::length(transf[0]);
    float yScale = glm::length(transf[1]);
    float zScale = glm::length(transf[2]);

    return glm::mat4(
        transf[0][0] / xScale, transf[0][1] / xScale, transf[0][2] / xScale, 0.0f,
        transf[1][0] / yScale, transf[1][1] / yScale, transf[1][2] / yScale, 0.0f,
        transf[2][0] / zScale, transf[2][1] / zScale, transf[2][2] / zScale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

void FakeShadow::updateTransform()
{
    if (this->getParent() != nullptr) {
        if (getCastShadow()) {
            //World position of orginal mesh
            glm::vec3 modelPos = model->getWorldPosition();
            //World position of node on which shadow will be casted
            glm::vec3 shadowParentPos = getParent()->getWorldPosition();
            shadowParentPos.y += shadowParentPos.y + 0.6f;

            offset = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(modelPos.x - shadowParentPos.x, shadowParentPos.y, modelPos.z - shadowParentPos.z)
            );

            glm::mat4 R = getRotationMatrix();
            glm::mat4 transf = model->getFinalMatrix();
            float xScale = glm::length(transf[0]);
            float yScale = glm::length(transf[1]);
            float zScale = glm::length(transf[2]);
            glm::mat4 modelScale = glm::scale(
                glm::mat4(1.0f),
                glm::vec3(xScale, yScale, zScale)
            );

            transf = this->getParent()->getFinalMatrix();
            xScale = glm::length(transf[0]);
            yScale = glm::length(transf[1]);
            zScale = glm::length(transf[2]);
            glm::mat4 shadowParentScale = glm::scale(
                glm::mat4(1.0f),
                glm::vec3(
                    1 / xScale,
                    1 / yScale,
                    1 / zScale
                )
            );

            setTransform(offset * scaling * shadowParentScale * modelScale * R);
        }
    }
}

void LIB_API FakeShadow::render(glm::mat4 finalMatrix) {

    if(this->getParent() != nullptr) {
        if (getCastShadow()) {
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
    }
}
