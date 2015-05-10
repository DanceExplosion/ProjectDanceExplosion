#include "TextureLoader.h"



CTexture::CTexture()
{
	bMipMapsGenerated = false;
}

// Loading in the selected texture
bool CTexture::loadTexture2D(string a_sPath, bool bGenerateMipMaps)
{
	// initialising DevIL libraries
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);	
	
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glGenSamplers(1, &uiSampler);

	// Load in texture
	if (ilLoadImage(a_sPath.c_str()))
	{
		// checking relevant data has been loaded
		ILubyte* data = ilGetData();
		if (!data)
			std::cout << "No image data found" << std::endl;
		else
		{
			std::cout << "image loaded" << std::endl;
			// create new texture enum for model
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &uiTexture);
			glBindTexture(GL_TEXTURE_2D, uiTexture);

			// texture wrapping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// texture mipmap generation? usage?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//bind data to textureRef
			uiTexture = ilutGLBindTexImage();
		}
	}
	else
	{
		// erro message
		std::cout << "failed to load image" << std::endl;
		ILenum error = ilGetError();
		std::cout << "error: " << iluErrorString(error) << std::endl;
	}

	// check that ptexture is created
	if (glIsTexture(uiTexture))
		std::cout << "texture success" << std::endl;

	// unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);

	return true; // Success
}

void CTexture::setSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(uiSampler, parameter, value);
}

void CTexture::setFiltering(int a_tfMagnification, int a_tfMinification)
{
	// Set magnification filter
	if (a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	tfMinification = a_tfMinification;
	tfMagnification = a_tfMagnification;
}

// Binding texture to the buffers
void CTexture::bindTexture(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
}

// Function to remove texture if need be
void CTexture::releaseTexture()
{
	glDeleteSamplers(1, &uiSampler);
	glDeleteTextures(1, &uiTexture);
}