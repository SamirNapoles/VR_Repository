#include "Texture.h"

#pragma once

class CubeMap : public Texture {
public:
	CubeMap(int id, const std::string name, std::string positiveX, std::string negativeX, std::string positiveY, std::string negativeY, std::string positiveZ, std::string negativeZ);
	~CubeMap();
	void init();
	// can be deleted?
	void render(glm::mat4 finalMatrix);
private:
	unsigned int cubeMapId;
	std::string cubeMapImages[6];
};