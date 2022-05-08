#pragma once
#include "Mesh.h"
#include "Leap.h"
#include <vector>



#ifndef HAND
#define HAND


class LIB_API Hands : public Mesh
{
public:
	Hands(const int id, const std::string name);
	~Hands();

public:
	void render(glm::mat4 finalMatrix) override;
	glm::vec3* getIndexPosition();

private:
	void buildSphere();

private:
	unsigned int sphereVao;
	unsigned int sphereVertexVbo;
	unsigned int sphereTextureVbo;
	std::vector<glm::vec3> sphereVertices;
	std::vector<glm::vec2> sphereTextures;
	Texture* texture;
	Leap* leap;
	glm::vec3 indexPosition[2];
};

#endif //HAND

