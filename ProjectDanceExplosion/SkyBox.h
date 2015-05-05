#include <string>
#include <windows.h>
#include <vector>
#include "TextureLoader.h"

#include <gl/glew.h>
#include <glm/glm.hpp>

class SkyBox
{
public:
	void loadSkybox(std::string a_sDirectory, std::string a_sFront, std::string a_sBack, std::string a_sLeft, std::string a_sRight, std::string a_sTop, std::string a_sBottom);
	void renderSkybox();
	GLuint getVertexArray();
private:
	GLuint uiVAO;
	CTexture tTextures[6];
	std::string sDirectory;
	std::string sFront, sBack, sLeft, sRight, sTop, sBottom;
	UINT uiBuffer;
	std::vector<BYTE> data;
	float skyboxSize;
};