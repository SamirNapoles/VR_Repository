#include "FileReader.h"
#include "OvObject.h"
#include "OvMesh.h"
#include "OvLight.h"
#include "Mesh.h"
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "FakeShadow.h"
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/packing.hpp>
#pragma warning(disable : 4996)

FileReader::FileReader() {}

FileReader::~FileReader() {
	materials.clear();
	tempVertices.clear();
}

Node* FileReader::readFile(const char* path) {

	// Open file:
	FILE* dat = fopen(path, "rb");
	if (dat == nullptr)
	{
		std::cout << "ERROR: unable to open file '" << path << "'" << std::endl;
		return nullptr;
	}

	Material* shadow_material = new Material(Object::getNextId(), "shadow_material", glm::vec4(glm::vec3(0.0f), 1.0f), glm::vec4(glm::vec3(0.0f), 1.0f), glm::vec4(glm::vec3(0.0f), 1.0f), glm::vec4(glm::vec3(0.0f), 1.0f), 0.0f);
	materials.insert(std::pair<std::string, Material*>(shadow_material->getName(), shadow_material));

	Node* root = recursiveLoad(dat);
	fclose(dat);

	return root;
}

// Recursive loading function
Node* FileReader::recursiveLoad(FILE* dat)
{
	// Parse the chunk starting at buffer + position:
	unsigned int chunkId, chunkSize, position = 0;

	fread(&chunkId, sizeof(unsigned int), 1, dat);
	fread(&chunkSize, sizeof(unsigned int), 1, dat);

	// Load whole chunk into memory:
	char* data = new char[chunkSize];
	if (fread(data, sizeof(char), chunkSize, dat) != chunkSize)
	{
		std::cout << "ERROR: unable to read from file" << std::endl;
		fclose(dat);
		delete[] data;
	}

	//First Object
	if ((OvObject::Type)chunkId == OvObject::Type::O_OBJECT) {
		unsigned int versionId;
		memcpy(&versionId, data + position, sizeof(unsigned int));
		position += sizeof(unsigned int);

		return recursiveLoad(dat);
	}

	//Material
	if ((OvObject::Type)chunkId == OvObject::Type::O_MATERIAL) {
		// Material name:
		char materialName[FILENAME_MAX];
		strcpy(materialName, data + position);
		position += (unsigned int)strlen(materialName) + 1;
		std::string materialName_str(materialName);

		// Material term colors, starting with emissive:
		glm::vec3 emission, albedo;
		memcpy(&emission, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);

		// Albedo:
		memcpy(&albedo, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);

		// Roughness factor:
		float roughness;
		memcpy(&roughness, data + position, sizeof(float));
		position += sizeof(float);

		//Skipped values:
		// Metalness factor:
		position += sizeof(float);
		// Transparency factor:
		position += sizeof(float);

		// Albedo texture filename, or [none] if not used:
		char textureName[FILENAME_MAX];
		strcpy(textureName, data + position);
		position += (unsigned int)strlen(textureName) + 1;
		std::string textureName_str = std::string(textureName);

		//Save material on data structure
		Material* material = new Material(Object::getNextId(), materialName_str, glm::vec4(emission, 1.0f), glm::vec4(albedo * 0.2f, 1.0f), glm::vec4(albedo * 0.6f, 1.0f), glm::vec4(albedo * 0.4f, 1.0f), (1 - sqrt(roughness)) * 128);
		materials.insert(std::pair<std::string, Material*>(material->getName(), material));

		Texture* texture = new Texture(Object::getNextId(), textureName_str);
		texture->setTexId(textureName_str);
		material->setTexture(texture);

		return recursiveLoad(dat);
	}

	// Node name:
	char nodeName[FILENAME_MAX];
	strcpy(nodeName, data + position);
	position += (unsigned int)strlen(nodeName) + 1;
	std::string nodeName_str(nodeName);

	// Node matrix:
	glm::mat4 matrix;
	memcpy(&matrix, data + position, sizeof(glm::mat4));
	position += sizeof(glm::mat4);

	// Nr. of children nodes:
	unsigned int nrOfChildren;
	memcpy(&nrOfChildren, data + position, sizeof(unsigned int));
	position += sizeof(unsigned int);

	// Parse chunk information according to its type:
	switch ((OvObject::Type)chunkId)
	{
	case OvObject::Type::O_NODE:
	{
		Node* thisNode = new Node(Object::getNextId(), nodeName_str);
		thisNode->setTransform(matrix);

		// Go recursive when child nodes are avaialble:
		if (nrOfChildren)
			while (thisNode->getNumberOfChildren() < nrOfChildren)
			{
				Node* childNode = recursiveLoad(dat);
				thisNode->addChild(childNode);
			}

		// Done:
		return thisNode;
	}

	case OvObject::Type::O_MESH:
	{
		// Optional target node, or [none] if not used:
		char targetName[FILENAME_MAX];
		strcpy(targetName, data + position);
		position += (unsigned int)strlen(targetName) + 1;

		// Mesh subtype (see OvMesh SUBTYPE enum):
		position += sizeof(unsigned char);

		// Material name, or [none] if not used:
		char materialName[FILENAME_MAX];
		strcpy(materialName, data + position);
		position += (unsigned int)strlen(materialName) + 1;
		std::string materialName_str(materialName);

		Mesh* thisMesh;
		if (materialName_str != "[none]") {
			std::shared_ptr<Material> material(materials.find(materialName_str)->second);
			thisMesh = new Mesh(Object::getNextId(), nodeName_str, material);
			thisMesh->setTransform(matrix);
		}
		else {
			thisMesh = new Mesh(Object::getNextId(), nodeName_str, nullptr);
			thisMesh->setTransform(matrix);
		}

		// Mesh bounding sphere radius:
		float radius;
		memcpy(&radius, data + position, sizeof(float));
		position += sizeof(float);
		// Mesh bounding box minimum corner:
		glm::vec3 bBoxMin;
		memcpy(&bBoxMin, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);
		// Mesh bounding box maximum corner:
		position += sizeof(glm::vec3);
		// Optional physics properties:
		unsigned char hasPhysics;
		memcpy(&hasPhysics, data + position, sizeof(unsigned char));
		position += sizeof(unsigned char);
		if (hasPhysics)
		{
			/**
			 * Mesh physics properties.
			 */
			struct PhysProps
			{
				// Pay attention to 16 byte alignement (use padding):
				unsigned char type;
				unsigned char contCollisionDetection;
				unsigned char collideWithRBodies;
				unsigned char hullType;

				// Vector data:
				glm::vec3 massCenter;

				// Mesh properties:
				float mass;
				float staticFriction;
				float dynamicFriction;
				float bounciness;
				float linearDamping;
				float angularDamping;
				unsigned int nrOfHulls;
				unsigned int _pad;

				// Pointers:
				void* physObj;
				void* hull;
			};

			PhysProps mp;
			memcpy(&mp, data + position, sizeof(PhysProps));
			position += sizeof(PhysProps);
			// Custom hull(s) used?
			if (mp.nrOfHulls)
			{
				for (unsigned int c = 0; c < mp.nrOfHulls; c++)
				{
					// Hull number of vertices:
					unsigned int nrOfVertices;
					memcpy(&nrOfVertices, data + position, sizeof(unsigned int));
					position += sizeof(unsigned int);
					// Hull number of faces:
					unsigned int nrOfFaces;
					memcpy(&nrOfFaces, data + position, sizeof(unsigned int));
					position += sizeof(unsigned int);
					// Hull centroid:
					position += sizeof(glm::vec3);
					// Iterate through hull vertices:
					for (unsigned int c = 0; c < nrOfVertices; c++)
					{
						position += sizeof(glm::vec3);
					}
					// Iterate through hull faces:
					for (unsigned int c = 0; c < nrOfFaces; c++)
					{
						position += sizeof(unsigned int) * 3;
					}
				}
			}
		}


		std::shared_ptr<Material> smaterial(materials.find("shadow_material")->second);
		FakeShadow* shadow = new FakeShadow(Object::getNextId(), nodeName_str + "_shadow", smaterial, thisMesh);


		// Nr. of LODs:
		unsigned int LODs;
		memcpy(&LODs, data + position, sizeof(unsigned int));
		position += sizeof(unsigned int);

		/*
		// For each LOD...:
		float offset = 0;
		for (unsigned int l = 0; l < LODs; l++)
		{
		*/

		// Nr. of vertices:
		unsigned int vertices, faces;
		memcpy(&vertices, data + position, sizeof(unsigned int));
		position += sizeof(unsigned int);

		// ...and faces:
		memcpy(&faces, data + position, sizeof(unsigned int));
		position += sizeof(unsigned int);

		glm::vec3* vertexPtr = new glm::vec3[vertices];
		glm::vec3* normalPtr = new glm::vec3[vertices];
		glm::vec2* texturePtr = new glm::vec2[vertices];
		unsigned int* facePtr = new unsigned int[faces * 3];



		// Interleaved and compressed vertex/normal/UV/tangent data:
		for (unsigned int c = 0; c < vertices; c++)
		{
			// Vertex coords:
			glm::vec3 vertex;
			memcpy(&vertex, data + position, sizeof(glm::vec3));
			position += sizeof(glm::vec3);

			// Vertex normal:
			unsigned int normalData;
			memcpy(&normalData, data + position, sizeof(unsigned int));
			position += sizeof(unsigned int);

			// Texture coordinates:
			unsigned int textureData;
			memcpy(&textureData, data + position, sizeof(unsigned int));
			position += sizeof(unsigned int);

			// Tangent vector:
			unsigned int tangentData;
			memcpy(&tangentData, data + position, sizeof(unsigned int));
			position += sizeof(unsigned int);



			glm::vec4 normal = glm::unpackSnorm3x10_1x2(normalData);
			glm::vec2 uv = glm::unpackHalf2x16(textureData);
			/*
			Vertex* newVertex = new Vertex(vertex, glm::vec3(normal.x, normal.y, normal.z));
			newVertex->setTextureCoordinates(glm::vec2(uv));
			tempVertices.push_back(newVertex);
			if (vertex.y < offset)
				offset = vertex.y;
			*/

			vertexPtr[c] = vertex;
			normalPtr[c] = glm::vec3(normal.x, normal.y, normal.z);
			texturePtr[c] = uv;	
		}

		// Faces:
		for (unsigned int c = 0; c < faces; c++)
		{
			// Face indexes:
			unsigned int face[3];
			memcpy(face, data + position, sizeof(unsigned int) * 3);
			position += sizeof(unsigned int) * 3;

			for (int i = 0; i < 3; i++)
			{
				//thisMesh->addVertex(tempVertices.at(face[i]), l);
				//shadow->addVertex(tempVertices.at(face[i]), l);
				facePtr[c * 3 + i] = face[i];
			}
		}

		tempVertices.clear();
		//}
		
		// VAO id:
		unsigned int vao;
		// Generate a vertex array object and bind it :
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// VBO id:
		unsigned int vertexVbo;
		// Generate a vertex buffer and bind it:
		glGenBuffers(1, &vertexVbo);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
		// Copy the vertex data from system to video memory:
		glBufferData(GL_ARRAY_BUFFER, vertices * sizeof(glm::vec3), vertexPtr, GL_STATIC_DRAW);

		// VBO id:
		unsigned int normalVbo;
		// Generate a vertex buffer and bind it:
		glGenBuffers(1, &normalVbo);
		glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
		// Copy the vertex data from system to video memory:
		glBufferData(GL_ARRAY_BUFFER, vertices * sizeof(glm::vec3), normalPtr, GL_STATIC_DRAW);

		// VBO id:
		unsigned int textureVbo;
		// Generate a vertex buffer and bind it:
		glGenBuffers(1, &textureVbo);
		glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
		// Copy the vertex data from system to video memory:
		glBufferData(GL_ARRAY_BUFFER, vertices * sizeof(glm::vec2), texturePtr, GL_STATIC_DRAW);

		// VBO id:
		unsigned int faceVbo;
		// Generate a vertex buffer and bind it:
		glGenBuffers(1, &faceVbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVbo);
		// Copy the vertex data from system to video memory:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces * 3 * sizeof(unsigned int), facePtr, GL_STATIC_DRAW);


		thisMesh->setVao(vertexVbo, normalVbo, textureVbo, faceVbo, vao, faces);

		// Go recursive when child nodes are avaialble:
		if (nrOfChildren)
			while (thisMesh->getNumberOfChildren() < nrOfChildren)
			{
				Node* childNode = recursiveLoad(dat);
				thisMesh->addChild(childNode);
			}

		shadow->setCastShadow(false);
		thisMesh->addChild(shadow);

		// Done:
		return thisMesh;
	}

	case OvObject::Type::O_LIGHT:
	{
		// Optional target node name, or [none] if not used:
		char targetName[FILENAME_MAX];
		strcpy(targetName, data + position);
		position += (unsigned int)strlen(targetName) + 1;

		// Light subtype (see OvLight SUBTYPE enum):
		unsigned char subtype;
		memcpy(&subtype, data + position, sizeof(unsigned char));
		char subtypeName[FILENAME_MAX];
		switch ((OvLight::Subtype)subtype)
		{
		case OvLight::Subtype::DIRECTIONAL: strcpy(subtypeName, "directional"); break;
		case OvLight::Subtype::OMNI: strcpy(subtypeName, "omni"); break;
		case OvLight::Subtype::SPOT: strcpy(subtypeName, "spot"); break;
		default: strcpy(subtypeName, "UNDEFINED");
		}
		position += sizeof(unsigned char);

		// Light color:
		glm::vec3 color;
		memcpy(&color, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);

		// Influence radius:
		float radius;
		memcpy(&radius, data + position, sizeof(float));
		position += sizeof(float);

		// Direction:
		glm::vec3 direction;
		memcpy(&direction, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);

		// Cutoff:
		float cutoff;
		memcpy(&cutoff, data + position, sizeof(float));
		position += sizeof(float);

		// Exponent:
		float spotExponent;
		memcpy(&spotExponent, data + position, sizeof(float));
		position += sizeof(float);

		// Cast shadow flag:
		unsigned char castShadows;
		memcpy(&castShadows, data + position, sizeof(unsigned char));
		position += sizeof(unsigned char);

		// Volumetric lighting flag:
		unsigned char isVolumetric;
		memcpy(&isVolumetric, data + position, sizeof(unsigned char));
		position += sizeof(unsigned char);

		Light* thisLight = nullptr;
		int nextLightPointer = Light::getNextLightNumber();
		switch ((OvLight::Subtype)subtype)
		{
		case OvLight::Subtype::DIRECTIONAL:
		{
			thisLight = new DirectionalLight(Object::getNextId(), nodeName_str, nextLightPointer, glm::vec4(glm::vec3(color.x), 1.0f), glm::vec4(glm::vec3(color.y), 1.0f), glm::vec4(glm::vec3(color.z), 1.0f));
		}break;
		case OvLight::Subtype::OMNI:
		{
			thisLight = new PointLight(Object::getNextId(), nodeName_str, nextLightPointer, glm::vec4(glm::vec3(color.x), 1.0f), glm::vec4(glm::vec3(color.y), 1.0f), glm::vec4(glm::vec3(color.z), 1.0f), cutoff);
		}break;
		case OvLight::Subtype::SPOT:
		{
			thisLight = new SpotLight(Object::getNextId(), nodeName_str, nextLightPointer, glm::vec4(glm::vec3(color.x), 1.0f), glm::vec4(glm::vec3(color.y), 1.0f), glm::vec4(glm::vec3(color.z), 1.0f), cutoff, direction);
		}break;
		default:
			thisLight = new Light(Object::getNextId(), nodeName_str, nextLightPointer, glm::vec4(glm::vec3(color.x), 1.0f), glm::vec4(glm::vec3(color.y), 1.0f), glm::vec4(glm::vec3(color.z), 1.0f));
		}


		thisLight->setTransform(matrix);

		// Go recursive when child nodes are avaialble:
		if (nrOfChildren)
			while (thisLight->getNumberOfChildren() < nrOfChildren)
			{
				Node* childNode = recursiveLoad(dat);
				thisLight->addChild(childNode);
			}
		// Done:
		return thisLight;
	}
	}
}
