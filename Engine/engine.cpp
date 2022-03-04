#include "Engine.h"
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

////////////////////////////////
// BODY OF CLASS Engine       //
////////////////////////////////
int Engine::windowId = 0;
List Engine::list = List();
void(*Engine::keyboardCallbackApplication)(int) = nullptr;
void(*Engine::displayCallBackApplication)() = nullptr;
Camera* Engine::camera = nullptr;
UIProjection* Engine::ui = nullptr;
FrameRate* Engine::fps = nullptr;

void LIB_API Engine::init(const char* windowName, void(*keyboardCallbackApplication)(int), void(*displayCallBackApplication)()) {
    // Init context:
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
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
    //Enable Z-Buffer
    glEnable(GL_DEPTH_TEST);
    //Enable face culling
    glEnable(GL_CULL_FACE);
    //Enable smooth shading
    glShadeModel(GL_SMOOTH);
    //Enable texture
    glEnable(GL_TEXTURE_2D);
    // Set callback functions:
    glutDisplayFunc(displayCallbackDelegator);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallbackDelegator);
    glutSpecialFunc(specialCallbackDelegator);
    Engine::keyboardCallbackApplication = keyboardCallbackApplication;
    Engine::displayCallBackApplication = displayCallBackApplication;
    //Initialize the UI
    ui = new UIProjection(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
    fps = new FrameRate();
}

void LIB_API Engine::free() {
    list.clear();
    FreeImage_DeInitialise();
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

    list.addEntry(root);
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
    // Refresh ui projection matrix:
    ui->setWidth(width);
    ui->setHeigth(height);
    ui->update();
    // Force rendering refresh:
    glutPostWindowRedisplay(windowId);
}

void Engine::displayCallbackDelegator() {
    displayCallBackApplication();
    fps->calculateFrameRate();
}

List LIB_API Engine::getList() {
    return Engine::list;
}

//Texture
void Engine::setTexturePath(std::string path)
{
    Texture::setPath(path);
}


//UI
UIProjection LIB_API* Engine::getUI() {
    return ui;
}

//Vector returned with size elements
// "width" and "height"

int LIB_API Engine::getFps() {
    return fps->getFps();
}