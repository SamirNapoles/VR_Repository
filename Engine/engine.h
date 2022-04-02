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
#include "Shader.h"
#include "Program.h"

/////////////
// CLASSES //
/////////////

/**
 * @brief Simple static class example.
 */
class LIB_API Engine
{
	public:
		Engine() {}
		~Engine() {}

	public:
		// Init/free:
		void init(const char* windowName, void(*keyboardCallbackApplication)(int), void(*displayCallBackApplication)());
		void free();

		Node* loadScene(std::string fileName);

		void clean(glm::vec4 color);
		void begin();
		static void swap();
		static void forceRefresh();

		static List* getList();
		static int getFps();
		static void setTexturePath(std::string width);
		void setCamera(Camera* camera);

		static Program* getProgramOmni();

		static Program* getProgramDirectional();

		static Program* getProgramSpot();

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
		static Node* root;
		static List list;
		static FrameRate* fps;
		// programOmni not used because omni lights considered spot light with 180� cut off 
		// (to make rendering work correctly avoiding unwanted buffer clearing when switching programs)
		static Program* programOmni;
		static Program* programDirectional;
		static Program* programSpot;
};