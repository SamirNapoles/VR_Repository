#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>

#include "Object.h"
#include "Shader.h"

class Program : public Object
{
public:
	static const unsigned int MAX_LOGSIZE = 4096;  ///< Max output size in char for a shader log

	// Const/dest:
	Program(int id, const std::string name);
	~Program();

	// Get/set:
	int getParamLocation(const char* name);
	void setMatrix(int param, const glm::mat4& mat);
	void setMatrix3(int param, const glm::mat3& mat);
	void setFloat(int param, float value);
	void setInt(int param, int value);
	void setBool(int param, bool value);
	void setVec3(int param, const glm::vec3& vect);
	void setVec4(int param, const glm::vec4& vect);

	bool build(Shader* vertexShader, Shader* fragmentShader);

	void bind(int location, const char* attribName);

	// Rendering:				
	bool render(void* data = nullptr);

	static Program* getActiveProgram();
	std::map<std::string, int> getUniforms();
	unsigned int getGlId();

private:
	unsigned int glId;
	static Program* activeProgram;
	std::map<std::string, int> uniformVariables;
};