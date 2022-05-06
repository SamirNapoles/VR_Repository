#include "LibDef.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef PROJECTION
#define PROJECTION

class LIB_API Projection {
	public:
		Projection(int width, int heigth);
		~Projection();

	public:
		void setOpenGLProjection();

		void setProjection(glm::mat4 projection);
		glm::mat4 getProjection();
		virtual void update();

		int getWidth();
		int getHeigth();
		
		void setWidth(int width);
		void setHeigth(int heigth);

		virtual float getFarPlane() = 0;
		virtual float getNearPlane() = 0;

	private:
		glm::mat4 projection{ 1.0f };
		int width;
		int heigth;
};

#endif //PROJECTION