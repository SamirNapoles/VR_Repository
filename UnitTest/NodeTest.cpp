#include "Test.h"
#include "Node.h"
#include "Object.h"

Node* node_1;
Node* node_2;
Node* node_3;
Node* node_4;
Node* node_5;
Node* node_6;
Node* node_7;
Node* node_8;

glm::mat4 translation = glm::translate(
    glm::mat4(1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f)
);
glm::mat4 rotation = glm::rotate(
    glm::mat4(1.0f),
    glm::radians(45.0f),
    glm::vec3(0.0f, 0.0f, 0.1f)
);
glm::mat4 scale = glm::scale(
    glm::mat4(1.0f),
    glm::vec3(0.5f)
);

Node* getNode(const int id) {
    return new Node(id, std::to_string(id));
}

void parentTest() {

    assert(node_1->getParent() == nullptr);

    assert(node_2->getParent()->getId() == node_1->getId());
    assert(node_4->getParent()->getId() == node_1->getId());
    assert(node_7->getParent()->getId() == node_1->getId());

    assert(node_3->getParent()->getId() == node_2->getId());

    assert(node_5->getParent()->getId() == node_4->getId());
    assert(node_6->getParent()->getId() == node_4->getId());

    assert(node_8->getParent()->getId() == node_7->getId());
}

void findTest() {

    assert(node_1->findById(8) != nullptr);

    assert(node_1->findByName("5") != nullptr);

    assert(node_2->findById(8) == nullptr);

    assert(node_1->findById(10) == nullptr);
}

void transformTest() {

    node_1->setTransform(translation);
    assert(node_1->getTransform() == translation);

    node_2->setTransform(rotation);
    assert(node_2->getTransform() == rotation);

    node_4->setTransform(rotation);
    assert(node_4->getTransform() == rotation);

    node_5->setTransform(scale);
    assert(node_5->getTransform() == scale);
}

void finalMatrixTest() {

    glm::mat4 t = translation;
    glm::mat4 r = rotation;
    glm::mat4 s = scale;
    assert(node_1->getFinalMatrix() == t);
    assert(node_2->getFinalMatrix() == t * r);
    assert(node_3->getFinalMatrix() == t * r);
    assert(node_4->getFinalMatrix() == t * r);
    assert(node_5->getFinalMatrix() == t * r * s);
    assert(node_6->getFinalMatrix() == t * r);
    assert(node_7->getFinalMatrix() == t);
    assert(node_8->getFinalMatrix() == t);
}

void removeTest() {

    node_1->removeById(4);
    assert(node_1->findById(4) == nullptr);
    assert(node_1->findById(5) == nullptr);
    assert(node_1->findById(6) == nullptr);

    node_7->removeByName("8");
    assert(node_7->findByName("8") == nullptr);
}

void nodeTest() {

    //       (1)
    //     /  |  \
    //  (2)  (4)   (7)
    //   |   / \    |
    //  (3) (5)(6) (8)
    node_1 = getNode(1);
    node_2 = getNode(2);
    node_3 = getNode(3);
    node_4 = getNode(4);
    node_5 = getNode(5);
    node_6 = getNode(6);
    node_7 = getNode(7);
    node_8 = getNode(8);

    node_1->addChild(node_2);
    node_1->addChild(node_4);
    node_1->addChild(node_7);
    node_2->addChild(node_3);
    node_4->addChild(node_5);
    node_4->addChild(node_6);
    node_7->addChild(node_8);

    parentTest();

    findTest();

    transformTest();

    finalMatrixTest();
    
    removeTest();
}