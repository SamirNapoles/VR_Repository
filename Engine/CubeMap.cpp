#include <GL/glew.h>
#include <FreeImage.h>
#include "CubeMap.h"

CubeMap::CubeMap(int id, const std::string name, std::string positiveX, std::string negativeX, std::string positiveY, std::string negativeY, std::string positiveZ, std::string negativeZ) :
    Texture{ id, name }, cubeMapImages { positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ }
{
}

CubeMap::~CubeMap()
{
    glDeleteTextures(1, &cubeMapId);
}

void CubeMap::init()
{
	/*if (cubeMapId)
	{
		glDeleteTextures(1, &cubeMapId);
	}*/

	glGenTextures(1, &cubeMapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	for (int i = 0; i < 6; i++)	// cubeMapImages->size() returns 8 ???
	{
		std::string cubeMap = Texture::getPath() + cubeMapImages[i];
		FIBITMAP* fibitmap = FreeImage_Load(FreeImage_GetFileType(cubeMap.c_str(), 0), cubeMap.c_str());
		if (fibitmap == nullptr)
			std::cout << "[ERROR] loading file '" << cubeMap << "'" << std::endl;

		FIBITMAP* fibitmap32bits = FreeImage_ConvertTo32Bits(fibitmap);

		// Fixing vertical/horizontal mirroring
		// FreeImage_FlipHorizontal(fibitmap32bits);
		FreeImage_FlipVertical(fibitmap32bits);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, FreeImage_GetWidth(fibitmap32bits), FreeImage_GetHeight(fibitmap32bits), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(fibitmap32bits));

		FreeImage_Unload(fibitmap);
		FreeImage_Unload(fibitmap32bits);
	}
}

void CubeMap::render(glm::mat4 finalMatrix)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
