#include "Engine.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage.h>

#include "VertexShader.h"
#include "FragmentShader.h"
//////////////
// DLL MAIN //
//////////////

#ifdef _WINDOWS
#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * DLL entry point. Avoid to rely on it for easier code portability (Linux doesn't use this method).
 * @param instDLL handle
 * @param reason reason
 * @param _reserved reserved
 * @return true on success, false on failure
 */
int APIENTRY DllMain(HANDLE instDLL, DWORD reason, LPVOID _reserved)
{
    // Check use:
    switch (reason)
    {
        ///////////////////////////
    case DLL_PROCESS_ATTACH: //
        break;
        ///////////////////////////
    case DLL_PROCESS_DETACH: //
        break;
    }
    // Done:
    return true;
}
#endif

/**
 * Debug message callback for OpenGL. See https://www.opengl.org/wiki/Debug_Output
 */
void __stdcall DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) {
    std::cout << "OpenGL says: \"" << std::string(message) << "\"" << std::endl;
}

////////////////////////////////
// BODY OF CLASS Engine       //
////////////////////////////////
int Engine::windowId = 0;
List Engine::list = List();
void(*Engine::keyboardCallbackApplication)(int) = nullptr;
void(*Engine::displayCallBackApplication)() = nullptr;
Camera* Engine::camera = nullptr;
FrameRate* Engine::fps = nullptr;
int Engine::screenW = 1024;
int Engine::screenH = 512;

Shader* vertexShader = nullptr;
Shader* fragmentShaderOmni = nullptr;
Program* Engine::programOmni = nullptr;
Shader* fragmentShaderDirectional = nullptr;
Program* Engine::programDirectional = nullptr;
Shader* fragmentShaderSpot = nullptr;
Program* Engine::programSpot = nullptr;

Shader* passthroughVertexShader = nullptr;
Shader* passthroughFragmentShader = nullptr;
Program* passthroughProgram = nullptr;

bool Engine::stereoscopic = false;
Projection* Engine::orthoProjection = nullptr;
Quad* Engine::quads[Fbo::EYE_LAST] = { nullptr, nullptr };
unsigned int Engine::quadTexId[Fbo::EYE_LAST] = { 0, 0 };
Fbo* Engine::quadFbo[Fbo::EYE_LAST] = { nullptr, nullptr };

SkyBox* Engine::skyBox = nullptr;

Engine::Engine(bool stereoscopic) {
    Engine::stereoscopic = stereoscopic;
}


void LIB_API Engine::init(const char* windowName, void(*keyboardCallbackApplication)(int), void(*displayCallBackApplication)()) {
    // Init context:
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(4, 4);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(Engine::screenW, Engine::screenH);

    // Init FreeImage:
    FreeImage_Initialise();

    // FreeGLUT can parse command-line params, in this case are just dummy values
    int argc = 0;
    char** argv = nullptr;
    glutInit(&argc, argv);
    // Set some optional flags:
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    // Create the window with a specific title:   
    Engine::windowId = glutCreateWindow(windowName);

    // The OpenGL context is now initialized...
    
    // Init all available OpenGL extensions (especially GLEW):
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        std::cout << "[ERROR] " << glewGetErrorString(error) << std::endl;
        return;
    }
    else {
        if (GLEW_VERSION_4_4)
            std::cout << "Driver supports OpenGL 4.4\n" << std::endl;
        else {
            std::cout << "[ERROR] OpenGL 4.4 not supported\n" << std::endl;
            return;
        }
    }

    // Register OpenGL debug callback:
    #ifdef _DEBUG
        glDebugMessageCallback((GLDEBUGPROC)DebugCallback, nullptr);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    #endif // _DEBUG

    /*********************************/

    // Log context properties:
    std::cout << "OpenGL properties:" << std::endl;
    std::cout << "   Vendor . . . :  " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "   Driver . . . :  " << glGetString(GL_RENDERER) << std::endl;

    int oglVersion[2];
    glGetIntegerv(GL_MAJOR_VERSION, &oglVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &oglVersion[1]);
    std::cout << "   Version  . . :  " << glGetString(GL_VERSION) << " [" << oglVersion[0] << "." << oglVersion[1] << "]" << std::endl;

    int oglContextProfile;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &oglContextProfile);
    if (oglContextProfile & GL_CONTEXT_CORE_PROFILE_BIT)
        std::cout << "                :  " << "Core profile" << std::endl;
    if (oglContextProfile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
        std::cout << "                :  " << "Compatibility profile" << std::endl;

    int oglContextFlags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &oglContextFlags);
    if (oglContextFlags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
        std::cout << "                :  " << "Forward compatible" << std::endl;
    if (oglContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
        std::cout << "                :  " << "Debug flag" << std::endl;
    if (oglContextFlags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT)
        std::cout << "                :  " << "Robust access flag" << std::endl;
    if (oglContextFlags & GL_CONTEXT_FLAG_NO_ERROR_BIT)
        std::cout << "                :  " << "No error flag" << std::endl;

    std::cout << "   GLSL . . . . :  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << std::endl;

    /*********************************/

    //Enable Z-Buffer
    glEnable(GL_DEPTH_TEST);
    //Enable face culling
    glEnable(GL_CULL_FACE);
    //Enable smooth shading
    glShadeModel(GL_SMOOTH);
    // Set callback functions:
    glutDisplayFunc(displayCallbackDelegator);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallbackDelegator);
    glutSpecialFunc(specialCallbackDelegator);
    glutCloseFunc(closeCallback);
    Engine::keyboardCallbackApplication = keyboardCallbackApplication;
    Engine::displayCallBackApplication = displayCallBackApplication;

    //shaders and program initialization here
    vertexShader = new Shader(Object::getNextId(), "vertx_shader");
    vertexShader->loadFromMemory(Shader::TYPE_VERTEX, VertexShader::vertexShader);

    fragmentShaderOmni = new Shader(Object::getNextId(), "fragment_shader_omni");
    fragmentShaderOmni->loadFromMemory(Shader::TYPE_FRAGMENT, FragmentShader::fragmentShaderOmni);

    programOmni = new Program(Object::getNextId(), "shader_program_omni");
    if (!programOmni->build(vertexShader, fragmentShaderOmni))
    {
        std::cout << "[ERROR] Unable to build program!" << std::endl;
        // exit(100);
    }
    if (!programOmni->render())
    {
        std::cout << "[ERROR] Unable to render program!" << std::endl;
        // exit(101);
    }
    programOmni->bind(0, "in_Position");
    programOmni->bind(1, "in_Normal");
    programOmni->bind(2, "in_TexCoord");

    fragmentShaderDirectional = new Shader(Object::getNextId(), "fragment_shader_directional");
    fragmentShaderDirectional->loadFromMemory(Shader::TYPE_FRAGMENT, FragmentShader::fragmentShaderDirectional);

    programDirectional = new Program(Object::getNextId(), "shader_program_directional");
    if (!programDirectional->build(vertexShader, fragmentShaderDirectional))
    {
        std::cout << "[ERROR] Unable to build program!" << std::endl;
        // exit(100);
    }
    if (!programDirectional->render())
    {
        std::cout << "[ERROR] Unable to render program!" << std::endl;
        // exit(101);
    }
    programDirectional->bind(0, "in_Position");
    programDirectional->bind(1, "in_Normal");
    programDirectional->bind(2, "in_TexCoord");

    fragmentShaderSpot = new Shader(Object::getNextId(), "fragment_shader_spot");
    fragmentShaderSpot->loadFromMemory(Shader::TYPE_FRAGMENT, FragmentShader::fragmentShaderSpot);

    programSpot = new Program(Object::getNextId(), "shader_program_spot");
    if (!programSpot->build(vertexShader, fragmentShaderSpot))
    {
        std::cout << "[ERROR] Unable to build program!" << std::endl;
        // exit(100);
    }
    if (!programSpot->render())
    {
        std::cout << "[ERROR] Unable to render program!" << std::endl;
        // exit(101);
    }
    programSpot->bind(0, "in_Position");
    programSpot->bind(1, "in_Normal");
    programSpot->bind(2, "in_TexCoord");

    passthroughVertexShader = new Shader(Object::getNextId(), "passthrough_vertx_shader");
    passthroughVertexShader->loadFromMemory(Shader::TYPE_VERTEX, VertexShader::passthroughVertexShader);

    passthroughFragmentShader = new Shader(Object::getNextId(), "passthrough_fragment_shader");
    passthroughFragmentShader->loadFromMemory(Shader::TYPE_FRAGMENT, FragmentShader::passthroughFragmentShader);

    passthroughProgram = new Program(Object::getNextId(), "shader_program_passthrough");
    if (!passthroughProgram->build(passthroughVertexShader, passthroughFragmentShader))
    {
        std::cout << "[ERROR] Unable to build program!" << std::endl;
        // exit(100);
    }
    if (!passthroughProgram->render())
    {
        std::cout << "[ERROR] Unable to render program!" << std::endl;
        // exit(101);
    }
    passthroughProgram->bind(0, "in_Position");
    passthroughProgram->bind(2, "in_TexCoord");

    glDepthFunc(GL_LEQUAL);

    //Initialize the UI
    fps = new FrameRate();

    skyBox = SkyBox::buildSkyBox("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg");
}

void LIB_API Engine::free() {
    list.clear();
    FreeImage_DeInitialise();
    //delete root; // avoid root memory leak
    delete vertexShader;
    delete fragmentShaderOmni;
    delete programOmni;
    delete fragmentShaderDirectional;
    delete programDirectional;
    delete fragmentShaderSpot;
    delete programSpot;
    delete passthroughVertexShader;
    delete passthroughFragmentShader;
    delete passthroughProgram;
}

void LIB_API Engine::setCamera(Camera* camera) {
    this->camera = camera;
}

Camera LIB_API* Engine::getCamera() {
    return camera;
}

Node LIB_API* Engine::loadScene(std::string fileName) {
    FileReader fileReader = FileReader();
    Node* root = fileReader.readFile(fileName.c_str());

    float w = Engine::stereoscopic ? this->screenW / 2.0f : this->screenW;

    //free camera
    Projection* proj = new PerspectiveProjection(w, this->screenH, 45.0f, 1.0f, 1000.0f);
    Camera* camera = new Camera(Object::getNextId(), std::string("freeCamera"), proj);
    root->addChild(camera);

    //stationary camera
    proj = new PerspectiveProjection(w, this->screenH, 45.0f, 1.0f, 1000.0f);
    camera = new Camera(Object::getNextId(), std::string("stationaryCamera"), proj);
    root->addChild(camera);
    this->camera = camera;

    //quad projection
    if (Engine::stereoscopic) {
        Engine::orthoProjection = new OrthogonalProjection(this->screenW, this->screenH, 0.0f, this->screenW, 0.0f, this->screenH, -1.0f, 1.0f);
        
        GLint prevViewport[4];
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        Engine::quads[Fbo::EYE_LEFT] = new Quad(Object::getNextId(), "Quad_L", Fbo::EYE_LEFT, this->screenW / 2.0f, this->screenH);
        Engine::quads[Fbo::EYE_RIGHT] = new Quad(Object::getNextId(), "Quad_R", Fbo::EYE_RIGHT, this->screenW / 2.0f, this->screenH);

        glViewport(0, 0, prevViewport[2], prevViewport[3]);
    }
    
    return root;
}

void LIB_API Engine::clean(glm::vec4 color) {
    // Clear the screen:
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LIB_API Engine::begin() {
    // Enter the main FreeGLUT processing loop:
    glutMainLoopEvent();
}

void LIB_API Engine::swap() {
    // Swap this context's buffer:     
    glutSwapBuffers();
}

void LIB_API Engine::forceRefresh() {
    // Force rendering refresh:
    glutPostWindowRedisplay(windowId);
}

void Engine::keyboardCallbackDelegator(unsigned char key, int x, int y) {
    keyboardCallbackApplication(key);
}

void Engine::specialCallbackDelegator(int code, int x, int y) {
    keyboardCallbackApplication(code + 128);
}

void Engine::reshapeCallback(int width, int height) {
    if (width != Engine::screenW || height != Engine::screenH)
        glutReshapeWindow(Engine::screenW, Engine::screenH);
}

void Engine::displayCallbackDelegator() {

    //Normal rendering
    if (!Engine::stereoscopic) {
        displayCallBackApplication();
        fps->calculateFrameRate();
    }
    //Stereoscopic rendering
    else {
        // Store the current viewport size:
        GLint prevViewport[4];
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        //Render scene as stereocopic
        for (int c = 0; c < Fbo::EYE_LAST; c++) {
            // Render into this FBO:
            Engine::quads[c]->getFbo()->render();

            displayCallBackApplication();
        }
        Fbo::disable();
        glViewport(0, 0, prevViewport[2], prevViewport[3]);

        //Render quads with related fbos as texture
        passthroughProgram->render();
        passthroughProgram->setMatrix(Program::getActiveProgram()->Program::getUniforms()["projection"], Engine::orthoProjection->getProjection());
        passthroughProgram->setMatrix(Program::getActiveProgram()->Program::getUniforms()["modelview"], glm::mat4(1.0f));
        passthroughProgram->setVec4(Program::getActiveProgram()->Program::getUniforms()["color"], glm::vec4(1.0f));

        for (int c = 0; c < Fbo::EYE_LAST; c++) {
            glBindTexture(GL_TEXTURE_2D, Engine::quads[c]->getTexId());
            Engine::quads[c]->render(glm::mat4(1.0f));
        }

        fps->calculateFrameRate();
    }

    // Swap this context's buffer:
    Engine::swap();

    // Force rendering refresh:
    Engine::forceRefresh();
}

void Engine::closeCallback() {
    // Release OpenGL resources (VBO, VAO, shaders) while the context is still available:
    /*glDeleteBuffers(1, &vertexVbo);
    glDeleteBuffers(1, &colorVbo);
    glDeleteVertexArrays(1, &globalVao);
    delete shader;
    delete fs;
    delete vs;*/
}

List LIB_API* Engine::getList() {
    return &Engine::list;
}

//Texture
void Engine::setTexturePath(std::string path)
{
    Texture::setPath(path);
}


int LIB_API Engine::getFps() {
    return fps->getFps();
}

Program* Engine::getProgramOmni() {
    return programOmni;
}

Program* Engine::getProgramDirectional() {
    return programDirectional;
}

Program* Engine::getProgramSpot() {
    return programSpot;
}

SkyBox* Engine::getSkyBox()
{
    return skyBox;
}
