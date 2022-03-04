#include "Object.h"
#include "Node.h"
#include <list>

#ifndef LIST
#define LIST

class LIB_API List : public  Object {
	public:
		List() {}
		void addEntry(Node* node);
		void render(glm::mat4 cameraMatrix) override;
		void clear();
		Node* getObject(int i);
		int size();

	private:
		std::list<Node*> objectsList;
};

#endif //LIST