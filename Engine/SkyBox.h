#include "Object.h"
#include "Shader.h"
#include "Program.h"
#include "CubeMap.h"

#pragma once

class LIB_API SkyBox : public Object {
public:
	static SkyBox* buildSkyBox(std::string positiveX, std::string negativeX, std::string positiveY, std::string negativeY, std::string positiveZ, std::string negativeZ);
	~SkyBox();

	static std::string getProgramName();

	void render(glm::mat4 finalMatrix) override;
private:
	SkyBox(std::string positiveX, std::string negativeX, std::string positiveY, std::string negativeY, std::string positiveZ, std::string negativeZ);
	static SkyBox* skyBox;
	static float vertices[];
	static unsigned short faces[];
	static Shader* skyBoxVertexShader;
	static Shader* skyBoxFragmentShader;
	static std::string programName;
	static Program* program;
	unsigned int vao;
	unsigned int vertexVbo;
	unsigned int facesVbo;
	CubeMap* cubeMap;
};