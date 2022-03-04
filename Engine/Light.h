#include "Node.h"
#include <algorithm>

#ifndef LIGHT
#define LIGHT

class LIB_API Light : public Node {

public:
	Light(const int id, const std::string name, const int lightNumber, const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular);
	~Light();

public:
	int getLightNumber();
	void setPosition();
	void setPosition(glm::vec4 pos);
	glm::vec4 getPosition();
	static int getNextLightNumber();
	virtual void render(glm::mat4 cameraInv) override;
	virtual void setTransform(glm::mat4 transform) override;

	float getConstantAttenuation();
	float getLinearAttenuation();
	float getQuadraticAttenuation();
	void setConstantAttenuation(float constantAttenuation);
	void setLinearAttenuation(float linearAttenuation);
	void setQuadraticAttenuation(float quadraticAttenuation);

private:
	static int nextNumber;
	int lightNumber;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 position{0.0f, 0.0f, 0.0f, 1.0f};

	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.0f;
	float quadraticAttenuation = 0.0f;
};

#endif //LIGHT