#pragma once

#include <GL/glew.h>

#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

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
	inline void setMatrix(int param, const glm::mat4& mat)
	{
		glUniformMatrix4fv(param, 1, GL_FALSE, glm::value_ptr(mat));
	}
	inline void setMatrix3(int param, const glm::mat3& mat)
	{
		glUniformMatrix3fv(param, 1, GL_FALSE, glm::value_ptr(mat));
	}
	inline void setFloat(int param, float value)
	{
		glUniform1f(param, value);
	}
	inline void setInt(int param, int value)
	{
		glUniform1i(param, value);
	}
	inline void setVec3(int param, const glm::vec3& vect)
	{
		glUniform3fv(param, 1, glm::value_ptr(vect));
	}
	inline void setVec4(int param, const glm::vec4& vect)
	{
		glUniform4fv(param, 1, glm::value_ptr(vect));
	}

	bool build(Shader* vertexShader, Shader* fragmentShader);

	inline void bind(int location, const char* attribName)
	{
		glBindAttribLocation(glId, location, attribName);
	}

	// Rendering:				
	bool render(void* data = nullptr);

private:
	GLuint glId;
};