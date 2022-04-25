#include <GL/glew.h>
#include "SkyBox.h"
#include "CubeMap.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "Engine.h"

SkyBox* SkyBox::skyBox = nullptr;
Shader* SkyBox::skyBoxVertexShader = nullptr;
Shader* SkyBox::skyBoxFragmentShader = nullptr;
std::string SkyBox::programName = "skybox_program";
Program* SkyBox::program = nullptr;

float SkyBox::vertices[] = // Vertex and tex. coords are the same
{
   -1.0f,  1.0f,  1.0f,
   -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
   -1.0f,  1.0f, -1.0f,
   -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
};

unsigned short SkyBox::faces[] =
{
   0, 1, 2,
   0, 2, 3,
   3, 2, 6,
   3, 6, 7,
   4, 0, 3,
   4, 3, 7,
   6, 5, 4,
   7, 6, 4,
   4, 5, 1,
   4, 1, 0,
   1, 5, 6,
   1, 6, 2,
};

//  paramters are images' names
SkyBox* SkyBox::buildSkyBox(std::string positiveX, std::string negativeX, std::string positiveY, std::string negativeY, std::string positiveZ, std::string negativeZ)
{
    if (skyBox == nullptr)  // singleton; if already one skybox defined, return that one
    {
        skyBoxVertexShader = new Shader(Object::getNextId(), "skybox_vertex_shader");
        skyBoxVertexShader->loadFromMemory(Shader::TYPE_VERTEX, VertexShader::skyBoxVertexShader);

        skyBoxFragmentShader = new Shader(Object::getNextId(), "skybox_fragment_shader");
        skyBoxFragmentShader->loadFromMemory(Shader::TYPE_FRAGMENT, FragmentShader::skyBoxFragmentShader);

        program = new Program(Object::getNextId(), programName);
        if (!program->build(skyBoxVertexShader, skyBoxFragmentShader))
        {
            std::cout << "[ERROR] Unable to build program!" << std::endl;
            // exit(100);
        }
        if (!program->render())
        {
            std::cout << "[ERROR] Unable to render program!" << std::endl;
            // exit(101);
        }
        program->bind(0, "in_Position");

        skyBox = new SkyBox(positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ);
    }

    return skyBox;
}

SkyBox::~SkyBox()
{
    glDeleteBuffers(1, &vertexVbo);
    glDeleteBuffers(1, &facesVbo);
    glDeleteVertexArrays(1, &vao);

    delete cubeMap;

    delete program;
    delete skyBoxVertexShader;
    delete skyBoxFragmentShader;
}

std::string SkyBox::getProgramName()
{
    return programName;
}

void SkyBox::render(glm::mat4 finalMatrix)
{
    //cubeMap->render(finalMatrix);   // rendering of cubemap images, maybe useless (?)

    program->render();  // use skybox shaders
    Engine::getCamera()->getProjection()->setOpenGLProjection();    // setting shader's projection matrix

    program->setMatrix(program->getUniforms()["modelview"], finalMatrix);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, sizeof(faces) / sizeof(unsigned short), GL_UNSIGNED_SHORT, nullptr);
    glEnable(GL_CULL_FACE);
}

SkyBox::SkyBox(std::string positiveX, std::string negativeX, std::string positiveY, std::string negativeY, std::string positiveZ, std::string negativeZ)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &facesVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

    cubeMap = new CubeMap(Object::getNextId(), "skybox_cubemap", positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ);
    cubeMap->init();
}
