#include "Program.h"

#include <GL/glew.h>

#include <GL/freeglut.h>

Program* Program::activeProgram = nullptr;
//std::map<std::string, int> Program::uniformVariables = {};

Program::Program(int id, const std::string name) : Object(id, name), glId{ 0 }
{}

Program::~Program()
{
	if (glId)
	{
		glDeleteProgram(glId);
	}
}

void Program::setMatrix(int param, const glm::mat4& mat)
{
	glUniformMatrix4fv(param, 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::setMatrix3(int param, const glm::mat3& mat)
{
	glUniformMatrix3fv(param, 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::setFloat(int param, float value)
{
	glUniform1f(param, value);
}

void Program::setInt(int param, int value)
{
	glUniform1i(param, value);
}

void Program::setVec3(int param, const glm::vec3& vect)
{
	glUniform3fv(param, 1, glm::value_ptr(vect));
}

void Program::setVec4(int param, const glm::vec4& vect)
{
	glUniform4fv(param, 1, glm::value_ptr(vect));
}

/**
 * Returns the param location given its variable name.
 * @param name variable name
 * @return location ID or -1 if not found
 */
int Program::getParamLocation(const char* name)
{
	// Safety net:
	if (name == nullptr)
	{
		std::cout << "[ERROR] Invalid params" << std::endl;
		return 0;
	}

	// Return location:
	int r = glGetUniformLocation(glId, name);
	if (r == -1)
		std::cout << "[ERROR] Param '" << name << "' not found" << std::endl;
	return r;
}

/**
 * Loads and compiles a full program.
 * @param vertexShader pointer to a vertex shader, or nullptr if not used
 * @param fragmentShader pointer to a fragment shader, or nullptr if not used
 * @return true/false on success/failure
 */
bool Program::build(Shader* vertexShader, Shader* fragmentShader)
{
	// Safety net:
	if (vertexShader && vertexShader->getType() != Shader::TYPE_VERTEX)
	{
		std::cout << "[ERROR] Invalid vertex shader passed" << std::endl;
		return false;
	}
	if (fragmentShader && fragmentShader->getType() != Shader::TYPE_FRAGMENT)
	{
		std::cout << " [ERROR] Invalid fragment shader passed" << std::endl;
		return false;
	}

	// Delete if already used:
	if (glId)
	{
		// On reload, make sure it was a program before:
		/*
		if (this->type != TYPE_PROGRAM)
		{
			std::cout << "[ERROR] Cannot reload a shader as a program" << std::endl;
			return false;
		}
		*/
		glDeleteProgram(glId);
	}

	// Create program:
	glId = glCreateProgram();
	if (glId == 0)
	{
		std::cout << "[ERROR] Unable to create program" << std::endl;
		return false;
	}

	// Bind vertex shader:
	if (vertexShader)
		glAttachShader(glId, vertexShader->getGlId());

	// Bind fragment shader:
	if (fragmentShader)
		glAttachShader(glId, fragmentShader->getGlId());

	// Link program:
	glLinkProgram(glId);
	//this->type = TYPE_PROGRAM;

	// Verify program:
	int status;
	char buffer[MAX_LOGSIZE];
	int length = 0;
	memset(buffer, 0, MAX_LOGSIZE);

	glGetProgramiv(glId, GL_LINK_STATUS, &status);
	glGetProgramInfoLog(glId, MAX_LOGSIZE, &length, buffer);
	if (status == false)
	{
		std::cout << "[ERROR] Program link error: " << buffer << std::endl;
		return false;
	}
	glValidateProgram(glId);
	glGetProgramiv(glId, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "[ERROR] Unable to validate program" << std::endl;
		return false;
	}

	uniformVariables["projection"] = this->getParamLocation("projection");
	uniformVariables["modelview"] = this->getParamLocation("modelview");
	uniformVariables["modelviewInverseTranspose"] = this->getParamLocation("modelviewInverseTranspose");

	uniformVariables["materialEmission"] = this->getParamLocation("materialEmission");
	uniformVariables["materialAmbient"] = this->getParamLocation("materialAmbient");
	uniformVariables["materialDiffuse"] = this->getParamLocation("materialDiffuse");
	uniformVariables["materialSpecular"] = this->getParamLocation("materialSpecular");
	uniformVariables["materialShiniess"] = this->getParamLocation("materialShiniess");

	// uniformVariables["lightPosition"] = this->getParamLocation("lightPosition");
	// uniformVariables["lightDirection"] = this->getParamLocation("lightDirection");
	uniformVariables["lightAmbient"] = this->getParamLocation("lightAmbient");
	uniformVariables["lightDiffuse"] = this->getParamLocation("lightDiffuse");
	uniformVariables["lightSpecular"] = this->getParamLocation("lightSpecular");
	// uniformVariables["cutOff"] = this->getParamLocation("cutOff");

	uniformVariables["color"] = this->getParamLocation("color");

	// Done:
	return true;
}

void Program::bind(int location, const char* attribName)
{
	glBindAttribLocation(glId, location, attribName);
}

/**
 * Shader rendering procedure.
 * @param data generic pointer to optional custom data
 * @return true on success, false on failure
 */
bool Program::render(void* data)
{
	// Activate shader:
	if (glId)
	{
		glUseProgram(glId);
		activeProgram = this;
	}
	else
	{
		std::cout << "[ERROR] Invalid shader rendered" << std::endl;
		return false;
	}

	// Done:
	return true;
}

Program* Program::getActiveProgram()
{
	return activeProgram;
}

std::map<std::string, int> Program::getUniforms()
{
	return uniformVariables;
}

unsigned int Program::getGlId()
{
	return glId;
}