#include "Object.h"
#include <vector>
#include <string>

#ifndef NODE
#define NODE

class LIB_API Node : public Object {

	public:
		Node(int id, const std::string name);
		Node(int id, const std::string name, Node* parent);
		~Node();

	public:
		void render(glm::mat4 cameraInv) override;

		Node* findById(const int id);
		Node* findByName(const std::string name);

		void removeById(const int id);
		void removeByName(const std::string name);
		void removeChildren();
		
		Node* getNthChild(int n) const;
		void addChild(Node* child);
		void removeNthChild(int n);
		int getNumberOfChildren() const;

		void setParent(Node* parent);
		Node* getParent() const;

		glm::mat4 getTransform() const;
		virtual void setTransform(glm::mat4 transform);
		glm::mat4 getFinalMatrix() const;

		glm::vec3 getWorldPosition() const;
		virtual void setWorldPosition(glm::vec3 position);

	private:
		std::vector<Node*> children;
		Node* parent;
		glm::mat4 transform = glm::mat4(1.0f);
};

#endif //NODE