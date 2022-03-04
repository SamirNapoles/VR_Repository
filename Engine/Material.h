#include "Object.h"
#include "Texture.h"

#ifndef MATERIAL
#define MATERIAL

class LIB_API Material : public Object {

public:
	Material(int id, const std::string name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess);
	Material(int id, const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess);
	~Material();

public:
	void render(glm::mat4 cameraInv) override;
	glm::vec4 getEmission();
	glm::vec4 getAmbient();
	glm::vec4 getDiffuse();
	glm::vec4 getSpecular();
	float getShininess();
	void setTexture(Texture* t);
	Texture* getTexture();

private:
	glm::vec4 emission;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;
	Texture* texture;
};

#endif //MATERIAL