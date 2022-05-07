#include "Hands.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Program.h"
#include "Engine.h"

Hands::Hands(const int id, const std::string name, std::shared_ptr<Material> material) :
	Mesh{ id, name, material } {

	buildSphere();
    setTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -20.0f, -70.0f)));
}

Hands::~Hands() {
    glDeleteBuffers(1, &sphereVao);
    glDeleteVertexArrays(1, &sphereVertexVbo);
    sphereVertices.clear();
}

void Hands::render(glm::mat4 finalMatrix) {

    //Material
    Material* m = material.get();
    m->render(finalMatrix);

    glBindVertexArray(sphereVao);

    // Render hands using spheres:
    const LEAP_TRACKING_EVENT* l = Engine::getLeap()->getCurFrame();
    for (unsigned int h = 0; h < l->nHands; h++) {

        LEAP_HAND hand = l->pHands[h];

        glm::mat4 f;
        glm::mat4 c;
        float scale = 0.1f;

        // Elbow:
        /*c = glm::translate(glm::mat4(1.0f), glm::vec3(hand.arm.prev_joint.x, hand.arm.prev_joint.y, hand.arm.prev_joint.z) * scale);
        f = finalMatrix * c;
        Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], f);
        Program::getActiveProgram()->setMatrix3(Program::getActiveProgram()->getUniforms()["modelviewInverseTranspose"], glm::inverseTranspose(glm::mat3(f)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)sphereVertices.size());*/

        // Wrist:
        c = glm::translate(glm::mat4(1.0f), glm::vec3(hand.arm.next_joint.x, hand.arm.next_joint.y, hand.arm.next_joint.z) * scale);
        f = finalMatrix * c;
        Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], f);
        Program::getActiveProgram()->setMatrix3(Program::getActiveProgram()->getUniforms()["modelviewInverseTranspose"], glm::inverseTranspose(glm::mat3(f)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)sphereVertices.size());

        // Palm:
        c = glm::translate(glm::mat4(1.0f), glm::vec3(hand.palm.position.x, hand.palm.position.y, hand.palm.position.z) * scale);
        f = finalMatrix * c;
        Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], f);
        Program::getActiveProgram()->setMatrix3(Program::getActiveProgram()->getUniforms()["modelviewInverseTranspose"], glm::inverseTranspose(glm::mat3(f)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)sphereVertices.size());

        // Distal ends of bones for each digit:
        for (unsigned int d = 0; d < 5; d++)
        {
            LEAP_DIGIT finger = hand.digits[d];
            for (unsigned int b = 0; b < 4; b++)
            {
                LEAP_BONE bone = finger.bones[b];
                c = glm::translate(glm::mat4(1.0f), glm::vec3(bone.next_joint.x, bone.next_joint.y, bone.next_joint.z) * scale);
                f = finalMatrix * c;
                Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], f);
                Program::getActiveProgram()->setMatrix3(Program::getActiveProgram()->getUniforms()["modelviewInverseTranspose"], glm::inverseTranspose(glm::mat3(f)));
                glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)sphereVertices.size());
            }
        }
    }

    glBindVertexArray(0);
}

void Hands::buildSphere() {

    // Build a sphere procedurally:   
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles        
    GLfloat radius = 1.0f;
    int gradation = 10;
    for (alpha = 0.0; alpha < glm::pi<float>(); alpha += glm::pi<float>() / gradation)
        for (beta = 0.0f; beta < 2.01f * glm::pi<float>(); beta += glm::pi<float>() / gradation)
        {
            x = radius * cos(beta) * sin(alpha);
            y = radius * sin(beta) * sin(alpha);
            z = radius * cos(alpha);
            sphereVertices.push_back(glm::vec3(x, y, z));
            x = radius * cos(beta) * sin(alpha + glm::pi<float>() / gradation);
            y = radius * sin(beta) * sin(alpha + glm::pi<float>() / gradation);
            z = radius * cos(alpha + glm::pi<float>() / gradation);
            sphereVertices.push_back(glm::vec3(x, y, z));
        }

    // Init buffers:   
    glGenVertexArrays(1, &sphereVao);
    glBindVertexArray(sphereVao);

    glGenBuffers(1, &sphereVertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVertexVbo);

    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(glm::vec3), sphereVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDisableVertexAttribArray(1);

    glDisableVertexAttribArray(2);
}