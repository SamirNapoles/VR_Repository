#include "Texture.h"
#include <GL/freeglut.h>
#include <FreeImage.h>

std::string Texture::path = "";
LIB_API Texture::Texture(int id, const std::string name) :
	Object{id, name}  {

	if (strstr((const char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic")) {
		isAnisotropicSupported = true;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicLevel);
	}
}

Texture::~Texture() {}


void LIB_API Texture::render(glm::mat4 cameraInv) {
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
	FreeImage_FlipVertical(bitmap);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,
			FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap),
			GL_BGRA_EXT, GL_UNSIGNED_BYTE,
			(void*)FreeImage_GetBits(bitmap));

	// Release bitmap
	FreeImage_Unload(bitmap);
}

void Texture::setPath(std::string path)
{
	Texture::path = path;
}


