#include "Program.h"

Program::Program(int id, const std::string name) : Object(id, name), glId{ 0 }
{}

Program::~Program()
{
	if (glId)
	{
		glDeleteProgram(glId);
	}
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
	if (vertexShader && vertexShader->getType() != vertexShader->TYPE_VERTEX)
	{
		std::cout << "[ERROR] Invalid vertex shader passed" << std::endl;
		return false;
	}
	if (fragmentShader && fragmentShader->getType() != fragmentShader->TYPE_FRAGMENT)
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

	// Done:
	return true;
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
		glUseProgram(glId);
	else
	{
		std::cout << "[ERROR] Invalid shader rendered" << std::endl;
		return false;
	}

	// Done:
	return true;
}
