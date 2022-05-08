#include "Object.h"
#include "Node.h"
#include "ListNode.h"
#include <list>

#ifndef LIST
#define LIST

class LIB_API List : public  Object {
	public:
		List() {}
		void addEntry(Node* node, glm::mat4 nodeMatrix);
		void render(glm::mat4 inverseCameraMatrix) override;
		void clear();
		void removeAllEntries();
		Node* getObject(int i);
		int size();
		std::vector<Node*> getElements();

	private:
		std::list<ListNode> objectsList;
};

#endif //LIST