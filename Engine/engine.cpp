#include "Engine.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage.h>

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

void LIB_API Engine::init(const char* windowName, void(*keyboardCallbackApplication)(int), void(*displayCallBackApplication)()) {
    // Init context:
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(4, 4);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(1000, 563);

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

    fps = new FrameRate();
}

void LIB_API Engine::free() {
    list.clear();
    FreeImage_DeInitialise();
    //delete root; // avoid root memory leak
}

void LIB_API Engine::setCamera(Camera* camera) {
    this->camera = camera;
}

Node LIB_API* Engine::loadScene(std::string fileName) {
    FileReader fileReader = FileReader();
    Node* root = fileReader.readFile(fileName.c_str());

    //free camera
    Projection* proj = new PerspectiveProjection(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), 45.0f, 1.0f, 1000.0f);
    Camera* camera = new Camera(Object::getNextId(), std::string("freeCamera"), proj);
    root->addChild(camera);

    //stationary camera
    proj = new PerspectiveProjection(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), 45.0f, 1.0f, 1000.0f);
    camera = new Camera(Object::getNextId(), std::string("stationaryCamera"), proj);
    root->addChild(camera);

    this->camera = camera;
    
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
    // Update viewport size:
    glViewport(0, 0, width, height);
    // Refresh projection matrix:
    Projection* p = camera->getProjection();
    p->setWidth(width);
    p->setHeigth(height);
    p->update();
    p->setOpenGLProjection();
    // Force rendering refresh:
    glutPostWindowRedisplay(windowId);
}

void Engine::displayCallbackDelegator() {
    displayCallBackApplication();
    fps->calculateFrameRate();
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