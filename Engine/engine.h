#include "LibDef.h"
#include "KeyCodes.h"
#include "FrameRate.h"
#include "Node.h"
#include "Projection.h"
#include "PerspectiveProjection.h"
#include "OrthogonalProjection.h"
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"
#include "Mesh.h"
#include "FakeShadow.h"
#include "FileReader.h"
#include "List.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "Shader.h"
#include "Program.h"
#include "Fbo.h"
#include "SkyBox.h"
#include "Quad.h"
#include "Eye.h"
#include "Leap.h"
#include "Hands.h"

/////////////
// CLASSES //
/////////////

/**
 * @brief Simple static class example.
 */
class LIB_API Engine
{
	public:
		Engine(bool stereoscopic = false);
		~Engine() {}

	public:
		// Init/free:
		void init(const char* windowName, void(*keyboardCallbackApplication)(int), void(*displayCallBackApplication)());
		static void free();

		Node* loadScene(std::string fileName);

		void clean(glm::vec4 color);
		void begin();
		static void swap();
		static void forceRefresh();

		static List* getList();
		static int getFps();
		static void setTexturePath(std::string width);
		void setCamera(Camera* camera);

		static Camera* getCamera();

		static Program* getProgramOmni();
		static Program* getProgramDirectional();
		static Program* getProgramSpot();
		static Program* getProgramPassThrough();

		static SkyBox* getSkyBox();

		static Leap* getLeap();

	private:
		static void keyboardCallbackDelegator(unsigned char key, int x, int y);
		static void specialCallbackDelegator(int code, int x, int y);
		static void(*keyboardCallbackApplication)(int);

	private:
		static void reshapeCallback(int width, int heigth);
		static void displayCallbackDelegator();
		static void(*displayCallBackApplication)();
		static void closeCallback();

	private:
		static int windowId;
		static Camera* camera;
		static List list;
		static FrameRate* fps;
		// programOmni not used because omni lights considered spot light with 180° cut off 
		// (to make rendering work correctly avoiding unwanted buffer clearing when switching programs)
		static Program* programOmni;
		static Program* programDirectional;
		static Program* programSpot;
		static Program* passthroughProgram;
		
		static int screenW;
		static int screenH;

		static bool stereoscopic;
		static Projection* orthoProjection;
		static Quad* quads[Eye::EYE_LAST];

		static SkyBox* skyBox;

		static Leap* leap;
};