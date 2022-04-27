#pragma once
#include "Mesh.h"
#include "Fbo.h"



#ifndef QUAD
#define QUAD


class LIB_API Quad : public Mesh
{
public:
	Quad(const int id, const std::string name, Fbo::Eye eye, int width, int height);
	~Quad();

public:
	void render(glm::mat4 finalMatrix) override;
	Fbo* getFbo();
	unsigned int getTexId();

private:
	void buildQuad();

private:
	glm::vec3* vertices;
	Fbo::Eye eye;
	int width;
	int height;
	unsigned int texId;
	Fbo* fbo;
};

#endif //QUAD

