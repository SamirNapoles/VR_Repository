#include "Light.h"

#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT

class LIB_API DirectionalLight : public Light {

public:
	DirectionalLight(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular);
	~DirectionalLight() {};

public:
	void render(glm::mat4 cameraInv) override;
	void setTransform(glm::mat4 transform) override;
};

#endif //DIRECTIONAL_LIGHT