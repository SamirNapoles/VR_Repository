#include "List.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <GL/freeglut.h>

#include <glm/gtx/string_cast.hpp>
#include "FakeShadow.h"

#include "engine.h"

void LIB_API List::addEntry(Node* root, glm::mat4 rootMatrix) {
    //don't get the root
    Node* node = root->getNthChild(0);
    int children = root->getNumberOfChildren();
    //insert nodes to list
    for (int i = 1; i <= children; i++) {
        //Add node
        Light* v;
        if ((v = dynamic_cast<Light*>(node))) {
            if(v->getLightNumber() <= GL_LIGHT7)
                objectsList.push_front(ListNode(node, rootMatrix * node->getTransform()));
        }
        else{
            FakeShadow* s;
            if (s = dynamic_cast<FakeShadow*>(node))
            {
                s->updateTransform();
            }
            objectsList.push_back(ListNode(node, rootMatrix * node->getTransform()));
        }
        //Add children of children
        if (node != NULL && node->getNumberOfChildren() > 0) {
            addEntry(node, rootMatrix * node->getTransform());
        }
        node = root->getNthChild(i);
    }
}

void LIB_API List::render(glm::mat4 inverseCameraMatrix) {
    std::list<ListNode>::iterator it;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        if (dynamic_cast<DirectionalLight*>((*it).getObject()) && (Program::getActiveProgram() != Engine::getProgramDirectional()))
            Engine::getProgramDirectional()->render();
        else if (dynamic_cast<SpotLight*>((*it).getObject()))
            Engine::getProgramSpot()->render();
        else if (dynamic_cast<PointLight*>((*it).getObject())) 
            Engine::getProgramOmni()->render();

        if (dynamic_cast<Light*>((*it).getObject()) == nullptr)
            break;

        Engine::getCamera()->getProjection()->setOpenGLProjection();
        (*it).getObject()->render(inverseCameraMatrix * (*it).getMatrix());

        // render only non-Light objects
        std::list<ListNode>::iterator nonLightIt;
        for (nonLightIt = objectsList.begin(); nonLightIt != objectsList.end(); nonLightIt++) {
            if (dynamic_cast<Light*>((*nonLightIt).getObject()) == nullptr) {
                (*nonLightIt).getObject()->render(inverseCameraMatrix * (*nonLightIt).getMatrix());
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
    }
    glDisable(GL_BLEND);
}

void List::clear()
{
    std::list<ListNode>::iterator it;
    //Delete each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        Light* v;
        if ((v = dynamic_cast<Light*>((*it).getObject()))) 
            delete (Light *)((*it).getObject());
        else
            delete (*it).getObject();
    }
    
    objectsList.clear();
}

void LIB_API List::removeAllEntries()
{
    objectsList.clear();
}

Node LIB_API *List::getObject(int i) {
    std::list<ListNode>::iterator it;
    int counter = 0;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        if (counter == i)
            return (*it).getObject();
        counter++;
    }
    return NULL;
}

int LIB_API List::size(){
    return objectsList.size();
}
