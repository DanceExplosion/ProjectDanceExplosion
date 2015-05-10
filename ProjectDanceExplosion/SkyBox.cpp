#include "SkyBox.h"

void SkyBox::loadSkybox(GLuint program, std::string a_sDirectory, std::string a_sFront, std::string a_sBack, std::string a_sLeft, std::string a_sRight, std::string a_sTop, std::string a_sBottom)
{
	shader = program;
	// Loads in the textures form the 6 sides
	tTextures[0].loadTexture2D(a_sDirectory + a_sFront);
	tTextures[1].loadTexture2D(a_sDirectory + a_sBack);
	tTextures[2].loadTexture2D(a_sDirectory + a_sLeft);
	tTextures[3].loadTexture2D(a_sDirectory + a_sRight);
	tTextures[4].loadTexture2D(a_sDirectory + a_sTop);
	tTextures[5].loadTexture2D(a_sDirectory + a_sBottom);

	sDirectory = a_sDirectory;

	sFront = a_sFront;
	sBack = a_sBack;
	sLeft = a_sLeft;
	sRight = a_sRight;
	sTop = a_sTop;
	sBottom = a_sBottom;
	
	// Effects how large the skybox is
	skyboxSize = 20.0f;

	for (int i = 0; i < 6; i++)
	{
		tTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
		tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	#pragma region Creating the Skybox
	glm::vec3 vSkyBoxVertices[24] =
	{
		// Front face
		glm::vec3(skyboxSize, skyboxSize, skyboxSize), glm::vec3(skyboxSize, -skyboxSize, skyboxSize), glm::vec3(-skyboxSize, skyboxSize, skyboxSize), glm::vec3(-skyboxSize, -skyboxSize, skyboxSize),
		// Back face
		glm::vec3(-skyboxSize, skyboxSize, -skyboxSize), glm::vec3(-skyboxSize, -skyboxSize, -skyboxSize), glm::vec3(skyboxSize, skyboxSize, -skyboxSize), glm::vec3(skyboxSize, -skyboxSize, -skyboxSize),
		// Left face
		glm::vec3(-skyboxSize, skyboxSize, skyboxSize), glm::vec3(-skyboxSize, -skyboxSize, skyboxSize), glm::vec3(-skyboxSize, skyboxSize, -skyboxSize), glm::vec3(-skyboxSize, -skyboxSize, -skyboxSize),
		// Right face
		glm::vec3(skyboxSize, skyboxSize, -skyboxSize), glm::vec3(skyboxSize, -skyboxSize, -skyboxSize), glm::vec3(skyboxSize, skyboxSize, skyboxSize), glm::vec3(skyboxSize, -skyboxSize, skyboxSize),
		// Top face
		glm::vec3(-skyboxSize, skyboxSize, -skyboxSize), glm::vec3(skyboxSize, skyboxSize, -skyboxSize), glm::vec3(-skyboxSize, skyboxSize, skyboxSize), glm::vec3(skyboxSize, skyboxSize, skyboxSize),
		// Bottom face
		glm::vec3(skyboxSize, -skyboxSize, -skyboxSize), glm::vec3(-skyboxSize, -skyboxSize, -skyboxSize), glm::vec3(skyboxSize, -skyboxSize, skyboxSize), glm::vec3(-skyboxSize, -skyboxSize, skyboxSize),
	};
	glm::vec2 vSkyBoxTexCoords[4] =
	{
		glm::vec2(0.0f, 1.0f), 
		glm::vec2(0.0f, 0.0f), 
		glm::vec2(1.0f, 1.0f), 
		glm::vec2(1.0f, 0.0f)
	};

	glm::vec3 vSkyBoxNormals[6] =
	{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	for (int i = 0; i < 24; i++)
	{
		data.insert(data.end(), (BYTE*)&vSkyBoxVertices[i], (BYTE*)&vSkyBoxVertices[i] + sizeof(glm::vec3));
		data.insert(data.end(), (BYTE*)&vSkyBoxTexCoords[i % 4], (BYTE*)&vSkyBoxTexCoords[i % 4] + sizeof(glm::vec2));
		data.insert(data.end(), (BYTE*)&vSkyBoxNormals[i / 4], (BYTE*)&vSkyBoxNormals[i / 4] + sizeof(glm::vec3));

	}
#pragma endregion
	
}

void SkyBox::renderSkybox(glm::mat4 view, glm::mat4 projection)
{
	//glGenVertexArrays(1, &uiVAO);
	//glBindVertexArray(uiVAO);
	
	glUseProgram(shader);

	GLuint modelVPId = glGetUniformLocation(shader, "VP");
	GLuint modelVPosId = glGetUniformLocation(shader, "vPos");
	glm::vec3 CameraPosition(glm::inverse(view)[3]);

	glUniform3f(modelVPosId, CameraPosition.x,CameraPosition.y,CameraPosition.z);
	glm::mat4 VP = projection*view;
	// USING SHADERS
	glUniformMatrix4fv(modelVPId, 1, GL_FALSE, &VP[0][0]);

	glGenBuffers(1, &uiBuffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, uiBuffer);

	glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);
	//data.clear();

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	


	glDepthMask(0);
	for (int i = 0; i < 6;i++)
	{
		tTextures[i].bindTexture();
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
	}

	glDepthMask(1);
	// disable texturing
	glBindTexture(GL_TEXTURE_2D, NULL);
	glActiveTexture(NULL);
	glDisable(GL_TEXTURE_2D);
	glDeleteBuffers(1, &uiBuffer);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glUseProgram(0);
}