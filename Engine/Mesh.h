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
	void addVertex(Vertex* v, int lod);
	virtual void render(glm::mat4 cameraInv) override;
	virtual bool getCastShadow() const;
	virtual void setCastShadow(bool castShadow);
	std::vector<Vertex*> getVertices(int lod);

protected:
	int lod = 0;
	std::vector<std::vector<Vertex*>> vertices;
	std::shared_ptr<Material> material;

private:
	bool castShadow = true;
};
#endif //MESH