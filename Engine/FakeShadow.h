#pragma once
#include "Mesh.h"
#include <math.h>



#ifndef FAKESHADOW
#define FAKESHADOW


class LIB_API FakeShadow : public Mesh
{
public:
	FakeShadow(const int id, const std::string name, std::shared_ptr<Material> material, Mesh* model);
	~FakeShadow();

public:
	void render(glm::mat4 cameraInv) override;
	void setOffset(glm::mat4 offset);
	glm::mat4 getOffset() const;
	glm::mat4 getScaling() const;
	void setShadowParent(Mesh* node);

private:
	glm::mat4 getRotationMatrix();

private:
	glm::mat4 offset;
	glm::mat4 scaling;
	Mesh* model;
};

#endif //FAKESHADOW

