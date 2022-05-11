#include "Hands.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Program.h"
#include "Engine.h"

Hands::Hands(const int id, const std::string name, const float height) :
	Mesh{ id, name, nullptr }, height(height) {

    leap = new Leap();
    if (!leap->init()) {
        std::cout << "[ERROR] Unable to init Leap Motion" << std::endl;
        delete leap;
        exit(101);
    }

	buildSphere();
    //setTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -0.4f)));

    texture = new Texture(Object::getNextId(), "hands_texture");
    texture->setTexId("[none]");
}

Hands::~Hands() {
    glDeleteBuffers(1, &sphereVao);
    glDeleteVertexArrays(1, &sphereVertexVbo);
    sphereVertices.clear();
    delete texture;

    // Free Leap:  
    leap->free();
    delete leap;
}

void Hands::render(glm::mat4 finalMatrix) {

    // Update Leap Motion status:
    leap->update();
    const LEAP_TRACKING_EVENT* l = leap->getCurFrame();

    //Temporary switch to passthrough shader program
    Program* prevProgram = Program::getActiveProgram();
    Engine::getProgramPassThrough()->render();
    Engine::getCamera()->getProjection()->setOpenGLProjection();
    Program::getActiveProgram()->setVec4(Program::getActiveProgram()->getUniforms()["color"], glm::vec4(1.0f));

    //Follow camera
    float cameraAltitude = Engine::getCamera()->getWorldPosition().y;
    glm::vec3 pos = glm::vec3(0.0f, cameraAltitude - (cameraAltitude - height), -0.4f);
    glm::mat4 m = glm::translate(glm::mat4(1.0f), pos);
    setTransform(m);

    glBindVertexArray(sphereVao);

    texture->render(finalMatrix);

    // Render hands using spheres:
    for (unsigned int h = 0; h < l->nHands; h++) {
        //leap->update();

        LEAP_HAND hand = l->pHands[h];

        glm::mat4 f;
        glm::mat4 c;
        float scale = 0.00125f;

        // Wrist:
        c = glm::translate(glm::mat4(1.0f), glm::vec3(hand.arm.next_joint.x, hand.arm.next_joint.y, hand.arm.next_joint.z) * scale);
        f = finalMatrix * c;
        Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)sphereVertices.size());

        // Palm:
        c = glm::translate(glm::mat4(1.0f), glm::vec3(hand.palm.position.x, hand.palm.position.y, hand.palm.position.z) * scale);
        f = finalMatrix * c;
        Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], f);
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
                glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)sphereVertices.size());

                //Save index finger position
                if (d == 1 && b == 3)
                    indexPosition[h] = (Engine::getCamera()->getFinalMatrix() * f)[3];
            }
        }
    }
    prevHandsNumber = l->nHands;

    glBindVertexArray(0);

    //Switch back to previous program in render pipeline
    prevProgram->render();
}

void Hands::buildSphere() {

    // Build a sphere procedurally:   
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles        
    GLfloat radius = 0.00625f;
    int gradation = 10;
    for (alpha = 0.0f; alpha < glm::pi<float>(); alpha += glm::pi<float>() / gradation)
        for (beta = 0.0f; beta < 2.01f * glm::pi<float>(); beta += glm::pi<float>() / gradation)
        {
            x = radius * cos(beta) * sin(alpha);
            y = radius * sin(beta) * sin(alpha);
            z = radius * cos(alpha);
            sphereVertices.push_back(glm::vec3(x, y, z));
            sphereTextures.push_back(glm::vec2(0.0f, 0.0f));

            x = radius * cos(beta) * sin(alpha + glm::pi<float>() / gradation);
            y = radius * sin(beta) * sin(alpha + glm::pi<float>() / gradation);
            z = radius * cos(alpha + glm::pi<float>() / gradation);
            sphereVertices.push_back(glm::vec3(x, y, z));
            sphereTextures.push_back(glm::vec2(0.0f, 0.0f));
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

    glGenBuffers(1, &sphereTextureVbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphereTextureVbo);
    glBufferData(GL_ARRAY_BUFFER, sphereTextures.size() * sizeof(glm::vec2), sphereTextures.data(), GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

std::vector<Node*> LIB_API Hands::getCollisions(Node* root) {

    std::vector<Node*> collisions;
    std::vector<Node*> elements = Engine::getList()->getElements();
    for (int i = 0; i < 2; i++) {
        glm::vec3 pos = indexPosition[i];

        std::vector<Node*>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++) {
            float distance = glm::distance(pos, (*it)->getWorldPosition());

            Mesh* obj;
            if ((obj = dynamic_cast<Mesh*>(*it)) && distance <= obj->getRadius()) {
                collisions.push_back(*it);
            }
        }
    }

    return collisions;
}