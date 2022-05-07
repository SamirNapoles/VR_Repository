#pragma once
#include "Mesh.h"
#include <vector>



#ifndef HAND
#define HAND


class LIB_API Hands : public Mesh
{
public:
	Hands(const int id, const std::string name, std::shared_ptr<Material> material);
	~Hands();

public:
	void render(glm::mat4 finalMatrix) override;

private:
	void buildSphere();

private:
	unsigned int sphereVao;
	unsigned int sphereVertexVbo;
	std::vector<glm::vec3> sphereVertices;
};

#endif //HAND

