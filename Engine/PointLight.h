#include "Light.h"

#ifndef POINT_LIGHT
#define POINT_LIGHT

class LIB_API PointLight : public Light {

public:
	PointLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular, const float cutOff = 180.0f);
	~PointLight() {};

public:
	virtual void render(glm::mat4 cameraInv) override;

private:
	float cutOff;
};

#endif //POINT_LIGHT