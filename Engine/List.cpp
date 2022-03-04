#include "List.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <GL/freeglut.h>

void LIB_API List::addEntry(Node* root) {
    //don't get the root
    Node* node = root->getNthChild(0);
    int children = root->getNumberOfChildren();
    //insert nodes to list
    for (int i = 1; i <= children; i++) {
        //Add node
        Light* v;
        if ((v = dynamic_cast<Light*>(node))) {
            if(v->getLightNumber() <= GL_LIGHT7)
                objectsList.push_front(node);
        }
        else{
            objectsList.push_back(node);
        }
        //Add children of children
        if (node != NULL && node->getNumberOfChildren() > 0) {
            addEntry(node);
        }
        node = root->getNthChild(i);
    }
}

void List::render(glm::mat4 cameraMatrix) {
    std::list<Node*>::iterator it;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        if (DirectionalLight* v = dynamic_cast<DirectionalLight*>(*it))
            v->render(cameraMatrix);
        if (PointLight* v = dynamic_cast<PointLight*>(*it))
            v->render(cameraMatrix);
        if (SpotLight* v = dynamic_cast<SpotLight*>(*it))
            v->render(cameraMatrix);
        else
            (*it)->render(cameraMatrix);
    }
}

void List::clear()
{
    std::list<Node*>::iterator it;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        Light* v;
        if ((v = dynamic_cast<Light*>(*it))) 
            delete (Light *)(*it);
        else
            delete (*it);
    }
    
    objectsList.clear();
}

Node LIB_API *List::getObject(int i) {
    std::list<Node*>::iterator it;
    int counter = 0;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        if (counter == i)
            return (*it);
        counter++;
    }
    return NULL;
}

int LIB_API List::size(){
    return objectsList.size();
}
