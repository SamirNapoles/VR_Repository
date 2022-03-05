#include "Object.h"
#include "Node.h"

#ifndef LIST_NODE
#define LIST_NODE

class LIB_API ListNode {
public:
	ListNode(Node* node, glm::mat4 nodeMatrix);
	Node* getObject();
	glm::mat4 getMatrix();

private:
	Node* object;
	glm::mat4 objectMatrix;
};

#endif //LIST_NODE