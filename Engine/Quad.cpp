#include "Quad.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Program.h"

Quad::Quad(const int id, const std::string name, Fbo::Eye eye, int width, int height) :
	Mesh{ id, name, nullptr }, eye(eye), width(width), height(height) {

    buildQuad();
}

void Quad::render(glm::mat4 finalMatrix) {
    
    // Set model matrix as current OpenGL matrix:
    Program::getActiveProgram()->setMatrix(Program::getActiveProgram()->getUniforms()["modelview"], finalMatrix);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, faceNr * 3);

    glBindVertexArray(0);
}

Fbo* Quad::getFbo() {
    return fbo;
}

unsigned int Quad::getTexId() {
    return texId;
}

void Quad::buildQuad() {

    glm::vec3 vertices[6] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(width, 0.0f, 0.0f),
        glm::vec3(0.0f, height, 0.0f),
        glm::vec3(0.0f, height, 0.0f),
        glm::vec3(width, 0.0f, 0.0f),
        glm::vec3(width, height, 0.0f)
    };

    for (int i = 0; i < 6; i++) {
        vertices[i].x += width * eye;
    }

    glm::vec2* texCoord = new glm::vec2[6];
    texCoord[0] = glm::vec2(0.0f, 0.0f);
    texCoord[1] = glm::vec2(1.0f, 0.0f);
    texCoord[2] = glm::vec2(0.0f, 1.0f);
    texCoord[3] = glm::vec2(0.0f, 1.0f);
    texCoord[4] = glm::vec2(1.0f, 0.0f);
    texCoord[5] = glm::vec2(1.0f, 1.0f);

    // VAO id:
    unsigned int vao;
    // Generate a vertex array object and bind it :
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO id:
    unsigned int vertexVbo;
    // Generate a vertex buffer and bind it:
    glGenBuffers(1, &vertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    // Copy the vertex data from system to video memory:
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDisableVertexAttribArray(1);

    // VBO id:
    unsigned int textureVbo;
    // Generate a vertex buffer and bind it:
    glGenBuffers(1, &textureVbo);
    glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
    // Copy the vertex data from system to video memory:
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), texCoord, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);

    this->setVao(vertexVbo, NULL, textureVbo, NULL, vao, 2);

    //Load FBO and its texture
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    fbo = new Fbo();
    fbo->bindTexture(0, Fbo::BIND_COLORTEXTURE, texId);
    fbo->bindRenderBuffer(1, Fbo::BIND_DEPTHBUFFER, width, height);
    if (!fbo->isOk())
        std::cout << "[ERROR] Invalid FBO" << std::endl;

    Fbo::disable();
}