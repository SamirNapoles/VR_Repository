#include "ListNode.h"

ListNode::ListNode(Node* node, glm::mat4 nodeMatrix) :
	object{node}, objectMatrix{nodeMatrix} {}

Node* ListNode::getObject()
{
	return object;
}

glm::mat4 ListNode::getMatrix()
{
	return objectMatrix;
}
