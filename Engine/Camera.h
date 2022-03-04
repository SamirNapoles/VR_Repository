#include "Node.h"
#include "Projection.h"

#ifndef CAMERA
#define CAMERA

class LIB_API Camera : public Node {
public:
	Camera(int id, const std::string name, Projection* projection);
	~Camera();

public:
	Projection* getProjection();
	glm::mat4 getInverse();

private:
	Projection* projection;
};

#endif //CAMERA