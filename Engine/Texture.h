#include "Object.h"
#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF

#ifndef TEXTURE
#define TEXTURE

class LIB_API Texture : public Object {

public:
	Texture(int id, const std::string name);
	~Texture();

public:
	void render(glm::mat4 cameraInv) override;
	void setTexId(std::string file);
	static void setPath(std::string);

private:
	unsigned int texId = 0;
	bool isAnisotropicSupported = false;
	int anisotropicLevel = 1;
	static std::string path;
};
#endif //TEXTURE
