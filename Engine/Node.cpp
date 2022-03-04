#include "Node.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

LIB_API Node::Node(int id, const std::string name, Node* parent) :
	Object(id, name),
	parent{ parent } {

	if (parent != nullptr)
		parent->addChild(this);
}

LIB_API Node::Node(int id, const std::string name) :
	Node(id, name, nullptr) {}

Node::~Node() {}

void LIB_API Node::render(glm::mat4 cameraInv) {
}

Node LIB_API *Node::findById(const int id) {
	if (id == this->getId())
		return this;

	int nrChildren = this->getNumberOfChildren();
	for (int i = 0; i < nrChildren; i++) {
		Node* child = this->getNthChild(i);
		Node* find = child->findById(id);

		if (find != nullptr) return find;
	}

	return nullptr;
}

Node LIB_API *Node::findByName(const std::string name)
{
	if (name == this->getName())
		return this;

	int nrChildren = this->getNumberOfChildren();
	for (int i = 0; i < nrChildren; i++) {
		Node* child = this->getNthChild(i);
		Node* find = child->findByName(name);

		if (find != nullptr) return find;
	}

	return nullptr;
}

void LIB_API Node::removeById(const int id) {

	Node* node = this->findById(id);

	if (node != nullptr) {
		node->removeChildren();

		Node* parent = node->getParent();
		if (parent != nullptr) {
			for (int i = 0; i < parent->getNumberOfChildren(); i++)
				if (parent->getNthChild(i)->getId() == node->getId())
					node->parent->removeNthChild(i);

			node->setParent(nullptr);
		}
	}
}

void LIB_API Node::removeByName(const std::string name) {

	Node* node = this->findByName(name);

	if (node != nullptr) {
		node->removeChildren();

		Node* parent = node->getParent();
		if (parent != nullptr) {
			for (int i = 0; i < parent->getNumberOfChildren(); i++)
				if (parent->getNthChild(i)->getId() == node->getId())
					node->parent->removeNthChild(i);

			node->setParent(nullptr);
		}
	}
}

void Node::removeChildren() {

	int nrChildren = this->getNumberOfChildren();
	for (int i = nrChildren - 1; i >= 0; i--) {
		Node* child = this->getNthChild(i);
		child->removeChildren();

		this->removeNthChild(i);
	}
}

Node* Node::getNthChild(int n) const
{
	if (children.empty() || n >= (int)children.size())
		return nullptr;

	return children.at(n);//.get();
}

void LIB_API Node::addChild(Node* child)
{
	if (child != nullptr) {
		child->parent = this;
		children.push_back(child);
	}
}

void Node::removeNthChild(int n)
{
	Node* child = getNthChild(n);
	if (child != nullptr) {
		child->setParent(nullptr);
		this->children.erase(children.begin() + n);
	}
}

int Node::getNumberOfChildren() const
{
	return (int)this->children.size();
}

void Node::setParent(Node* parent)
{
	this->parent = parent;
	if (parent != nullptr)
		parent->addChild(this);
}

Node LIB_API *Node::getParent() const
{
	return parent;
}

glm::mat4 LIB_API Node::getTransform() const
{
	return transform;
}

void LIB_API Node::setTransform(glm::mat4 transform)
{
	Node::transform = transform;
}

glm::mat4 LIB_API Node::getFinalMatrix() const {

	Node* par = parent;
	std::vector<Node*> parents;
	while (par != nullptr) {
		parents.push_back(par);
		par = par->getParent();
	}

	glm::mat4 m = glm::mat4(1.0f);
	for (int i = parents.size() - 1; i >= 0; i--) {
		par = parents.at(i);
		m *= par->getTransform();
	}
	m *= transform;

	return m;
}

glm::vec3 LIB_API Node::getWorldPosition() const {
	return glm::vec3(getFinalMatrix()[3]);
}
void LIB_API Node::setWorldPosition(glm::vec3 position) {
	transform[3] = glm::vec4(position, 1.0f);
}
