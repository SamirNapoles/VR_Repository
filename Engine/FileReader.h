#include "Node.h"
#include "Material.h"
#include "Vertex.h"
#include <string>
#include <map>



#ifndef FILEREADER
#define FILEREADER

class LIB_API FileReader  {
public:
	FileReader();
	~FileReader();

public:
	Node* readFile(const char* path);

private:
	Node* recursiveLoad(FILE* dat);

private:
	std::map<std::string, Material*> materials;
	std::vector<Vertex*> tempVertices;
};

#endif //FILEREADER
