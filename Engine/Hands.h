#pragma once
#include "Mesh.h"
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

private:
	void buildSphere();

private:
	unsigned int sphereVao;
	unsigned int sphereVertexVbo;
	unsigned int sphereTextureVbo;
	std::vector<glm::vec3> sphereVertices;
	std::vector<glm::vec2> sphereTextures;
	Texture* texture;
};

#endif //HAND

