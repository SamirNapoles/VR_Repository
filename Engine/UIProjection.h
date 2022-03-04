#include "OrthogonalProjection.h"
#include <vector>
#include <string>

#ifndef UI_PROJECTION
#define UI_PROJECTION

class LIB_API UIProjection : public OrthogonalProjection {
public:
	UIProjection(int width, int heigth);
	UIProjection(int width, int heigth, float left, float right, float bottom, float top, float zNear, float zFar);
	~UIProjection();

public:
	void setColor(glm::vec3 color);
	void addLabel(std::string label);
	void editLabel(int index, std::string label);
	void clear();
	void print();

private:
	std::vector<std::string> labels;
	glm::vec3 color{1.0f};
};

#endif //UI_PROJECTION