#include "Texture.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage.h>

std::string Texture::path = "";
LIB_API Texture::Texture(int id, const std::string name) :
	Object{id, name}  {

	GLint num = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &num);
	while (0 < --num) {
		if (strstr((const char*)glGetStringi(GL_EXTENSIONS, num - 1), "GL_EXT_texture_filter_anisotropic")) {
			isAnisotropicSupported = true;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicLevel);
		}
	}
}

Texture::~Texture() { glDeleteTextures(1, &texId); }


void LIB_API Texture::render(glm::mat4 finalMatrix) {
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);

	// Set circular coordinates:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set anisotropic filtering:
	if (isAnisotropicSupported)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicLevel);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);

	// Set trilinear filter and mipmapping:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void LIB_API Texture::setTexId(std::string file) {
	if (texId)
		glDeleteTextures(1, &texId);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	std::string filePath = Texture::path + file;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(filePath.c_str(), 0), filePath.c_str());
	FIBITMAP* bitmap32bits = FreeImage_ConvertTo32Bits(bitmap);
	FreeImage_FlipVertical(bitmap32bits);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,
			FreeImage_GetWidth(bitmap32bits), FreeImage_GetHeight(bitmap32bits),
			GL_BGRA_EXT, GL_UNSIGNED_BYTE,
			(void*)FreeImage_GetBits(bitmap32bits));

	// Release bitmap
	FreeImage_Unload(bitmap);
	FreeImage_Unload(bitmap32bits);
}

std::string Texture::getPath()
{
	return path;
}

void Texture::setPath(std::string path)
{
	Texture::path = path;
}


