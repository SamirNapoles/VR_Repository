#include "Node.h"
#include "Vertex.h"
#include "Material.h"

#ifndef MESH
#define MESH

class LIB_API Mesh : public Node {

public:
	Mesh(const int id, const std::string name, std::shared_ptr<Material> material);
	~Mesh();

public:
	void setVao(unsigned int vertexVbo, unsigned int normalVbo, unsigned int textureVbo, unsigned int faceVbo, unsigned int vao, unsigned int  faceNr);
	//void addVertex(Vertex* v, int lod);
	virtual void render(glm::mat4 finalMatrix) override;
	virtual bool getCastShadow() const;
	virtual void setCastShadow(bool castShadow);
	//std::vector<Vertex*> getVertices(int lod);
	float getRadius();
	void setRadius(float radius);

protected:
	int lod = 0;
	//std::vector<std::vector<Vertex*>> vertices;
	std::shared_ptr<Material> material;

	unsigned int vertexVbo;
	unsigned int normalVbo;
	unsigned int textureVbo;
	unsigned int faceVbo;
	unsigned int vao;
	unsigned int faceNr;

	float radius;

private:
	bool castShadow = true;
};
#endif //MESH