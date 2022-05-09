#include "List.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <GL/freeglut.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
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
    // getting far plane and near plane for sphere culling
    float farPlane{ Engine::getCamera()->getProjection()->getFarPlane() };
    float nearPlane{ Engine::getCamera()->getProjection()->getNearPlane() };

    float midPointZ{ nearPlane + (farPlane - nearPlane) / 2 };
    glm::vec4 midPoint = glm::inverse(inverseCameraMatrix) * glm::vec4(0.0f, 0.0f, -midPointZ, 1.0f);
    double boundingSphereRadius{ (farPlane - nearPlane) * sqrt(3) / 2 };

    glm::mat4 controlMatrix{ Engine::getCamera()->getProjection()->getProjection() * inverseCameraMatrix };
    glm::vec4 leftPlane = glm::row(controlMatrix, 3) + glm::row(controlMatrix, 0);
    glm::vec4 rightPlane = glm::row(controlMatrix, 3) - glm::row(controlMatrix, 0);
    glm::vec4 lowerPlane = glm::row(controlMatrix, 3) + glm::row(controlMatrix, 1);
    glm::vec4 upperPlane = glm::row(controlMatrix, 3) - glm::row(controlMatrix, 1);
    glm::vec4 nearerPlane = glm::row(controlMatrix, 3) + glm::row(controlMatrix, 2);
    glm::vec4 furtherPlane = glm::row(controlMatrix, 3) - glm::row(controlMatrix, 2);

    std::list<ListNode>::iterator it;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        if (dynamic_cast<DirectionalLight*>((*it).getObject()) && (Program::getActiveProgram() != Engine::getProgramDirectional()))
            Engine::getProgramDirectional()->render();
        else if (dynamic_cast<SpotLight*>((*it).getObject()) && (Program::getActiveProgram() != Engine::getProgramSpot()))
            Engine::getProgramSpot()->render();
        else if (dynamic_cast<PointLight*>((*it).getObject()) && (Program::getActiveProgram() != Engine::getProgramOmni())) 
            Engine::getProgramOmni()->render();
        
        if (dynamic_cast<Light*>((*it).getObject()) == nullptr)
            break;

        Engine::getCamera()->getProjection()->setOpenGLProjection();

        (*it).getObject()->render(inverseCameraMatrix * (*it).getMatrix());
        // render only non-Light objects
        std::list<ListNode>::iterator nonLightIt;
        for (nonLightIt = objectsList.begin(); nonLightIt != objectsList.end(); nonLightIt++) {
            if (dynamic_cast<Light*>((*nonLightIt).getObject()) == nullptr) {

                //Frustum culling
                Mesh* obj;
                float distance = glm::distance((*nonLightIt).getMatrix()[3], midPoint);
                if ((obj = dynamic_cast<Mesh*>((*nonLightIt).getObject())) && distance <= (boundingSphereRadius + obj->getRadius())) {

                    glm::vec4 pos = (*nonLightIt).getMatrix()[3];

                    glm::vec4 leftPoint     = glm::translate(glm::mat4(1.0f), glm::normalize(glm::vec3(rightPlane)  ) * obj->getRadius()) * pos;
                    glm::vec4 rightPoint    = glm::translate(glm::mat4(1.0f), glm::normalize(glm::vec3(leftPlane)   ) * obj->getRadius()) * pos;
                    glm::vec4 upPoint       = glm::translate(glm::mat4(1.0f), glm::normalize(glm::vec3(lowerPlane)  ) * obj->getRadius()) * pos;
                    glm::vec4 downPoint     = glm::translate(glm::mat4(1.0f), glm::normalize(glm::vec3(upperPlane)  ) * obj->getRadius()) * pos;
                    glm::vec4 farPoint      = glm::translate(glm::mat4(1.0f), glm::normalize(glm::vec3(nearerPlane) ) * obj->getRadius()) * pos;
                    glm::vec4 nearPoint     = glm::translate(glm::mat4(1.0f), glm::normalize(glm::vec3(furtherPlane)) * obj->getRadius()) * pos;

                    if ((glm::dot(rightPlane, leftPoint) >= 0) && (glm::dot(leftPlane, rightPoint) >= 0) && (glm::dot(lowerPlane, upPoint) >= 0)
                        && (glm::dot(upperPlane, downPoint) >= 0) && (glm::dot(nearerPlane, farPoint) >= 0) && (glm::dot(furtherPlane, nearPoint) >= 0)) {
                        obj->render(inverseCameraMatrix * (*nonLightIt).getMatrix());
                    }
                }
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
    }
    glDisable(GL_BLEND);

    // render skybox
    Engine::getSkyBox()->render((glm::translate(glm::mat4(1.0f), glm::vec3(-inverseCameraMatrix[3])) * inverseCameraMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1 / sqrt(3) * 0.9f * farPlane))));
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

std::vector<Node*> LIB_API List::getElements() {
    
    std::vector<Node*> elements;
    std::list<ListNode>::iterator it;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        elements.push_back((*it).getObject());
    }
    return elements;
}