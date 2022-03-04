#include "Test.h"
#include "List.h"
#include "Mesh.h"
#include "PointLight.h"

List list = List();
void listTest() {
	Node* root = new Node(Object::getNextId(), std::string("root"));
    //Material
    Material* materialWhite = new Material(Object::getNextId(), std::string("white"),
        glm::vec4(1.0f),	//emission
        glm::vec4(1.0f),	//ambient
        glm::vec4(1.0f),	//diffuse
        glm::vec4(1.0f),	//specular
        1					//shininess
    );
    Mesh* bulb = new Mesh(Object::getNextId(), std::string("bulb"), std::shared_ptr<Material>{materialWhite});

    PointLight* light = new PointLight(Object::getNextId(), std::string("light"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    Mesh* cube1 = new Mesh(Object::getNextId(), std::string("cube1"), std::shared_ptr<Material>{materialWhite});
    Mesh* cube2 = new Mesh(Object::getNextId(), std::string("cube2"), std::shared_ptr<Material>{materialWhite});
    root->addChild(bulb);
    bulb->addChild(light);
    root->addChild(cube1);
    root->addChild(cube2);
    list.addEntry(root);
    //4 elements in the list
    assert(list.size() == 4);

    //Check each element in list
    assert(list.getObject(0)->getName() == "light");
    assert(list.getObject(1)->getName() == "bulb");
    assert(list.getObject(2)->getName() == "cube1");
    assert(list.getObject(3)->getName() == "cube2");

    list.clear();
    assert(list.size() == 0);

    //Test to add more then 8 lights
    root = new Node(Object::getNextId(), std::string("root"));
    PointLight* light0 = new PointLight(Object::getNextId(), std::string("light0"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    PointLight* light1 = new PointLight(Object::getNextId(), std::string("light1"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    PointLight* light2 = new PointLight(Object::getNextId(), std::string("light2"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    PointLight* light3 = new PointLight(Object::getNextId(), std::string("light3"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    PointLight* light4 = new PointLight(Object::getNextId(), std::string("light4"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    PointLight* light5 = new PointLight(Object::getNextId(), std::string("light5"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    PointLight* light6 = new PointLight(Object::getNextId(), std::string("light6"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    PointLight* light7 = new PointLight(Object::getNextId(), std::string("light7"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );
    PointLight* light8 = new PointLight(Object::getNextId(), std::string("light8"), Light::getNextLightNumber(),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//ambient
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//diffuse
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	//specular
    );

    root->addChild(light0);
    root->addChild(light1);
    root->addChild(light2);
    root->addChild(light3);
    root->addChild(light4);
    root->addChild(light5);
    root->addChild(light6);
    root->addChild(light7);
    root->addChild(light8);

    list.addEntry(root);
    assert(list.size() == 8);

}
